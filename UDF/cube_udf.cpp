/*
  returns the cube of the values in a distribution

  input parameters:
	select ccube(Measure1,Measure2,...,Dimension) from Q4;

  output examples:
	Point Queries:
        select ccube_query("*");
        select ccube_query("M1,"M2","M3"...);
    Range Queries:
        select ccube_query("m12 m13,m21 m23,m31 m32");

  registering the function:
	CREATE AGGREGATE FUNCTION ccube RETURNS INTEGER SONAME 'ccube_udf.so';
	CREATE FUNCTION ccube_query RETURNS STRING SONAME 'ccube_udf.so';

  getting rid of the function:
  DROP FUNCTION ccube;
  DROP FUNCTION ccube_query;

*/

#include <QCTree.h>

#ifdef STANDARD
#include <stdio.h>
#include <string.h>

#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/

#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>

#ifdef HAVE_DLOPEN

#define BUFFERSIZE 1024

extern "C" {
    my_bool ccube_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
    void ccube_deinit( UDF_INIT* initid );
    void ccube_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
    void ccube_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
    double ccube( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );

    my_bool ccube_query_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
    void ccube_query_deinit( UDF_INIT* initid );
    char* ccube_query( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
}


struct ccube_data
{
    unsigned int dimensions;
    unsigned int measures;
    QCTree* tree;
    char* aggValues;
};

my_bool ccube_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{

    ccube_data *buffer = new ccube_data;
    buffer->tree = new QCTree();
    buffer->dimensions = args->arg_count-1;
    buffer->measures=1;

    for(int i = 0; i < args->arg_count-1; i++)
    {
        buffer->tree->baseTable->addCol("C"+i);
    }

    buffer->tree->baseTable->addMeasure("M");

    initid->maybe_null	= 1;
    initid->max_length	= 32;
    initid->ptr = (char*)buffer;

    return 0;
}

void ccube_deinit( UDF_INIT* initid )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;

    delete buffer->tree;
    delete buffer;
}

void ccube_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{

}

void ccube_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;
    vector<string> v;
    vector<double> m;
    int i=0;
    for( ; i < buffer->dimensions ; i++)
    {
        char* s = (char*) malloc(args->lengths[i]);
        memcpy(s,args->args[i], args->lengths[i]);
        s[args->lengths[i]] = '\0';
        v.push_back(s);
    }
    for( ; i < (buffer->dimensions + buffer->measures); i++)
    {
        m.push_back(*((double *)args->args[i]));
    }
    
    CubeTableRow row;
    row.dimensions = v;
    row.measures = m;
    buffer->tree->baseTable->addRow(row);
}

double ccube( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;
    buffer->tree->constructTree();
    buffer->tree->serialize();
}


my_bool ccube_query_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
    ccube_data *buffer = new ccube_data;
    buffer->tree = new QCTree();

    initid->maybe_null	= 1;
    initid->max_length	= 80;

    buffer->aggValues = (char*) malloc(80);
    bzero(buffer->aggValues,80);
    buffer->aggValues[79] = '\0';

    initid->ptr = (char*)buffer;

    return 0;
}

void ccube_query_deinit( UDF_INIT* initid )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;

    delete buffer->tree->qctree;
    delete buffer->aggValues;

    delete buffer;

}

char* ccube_query( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;
    QCTree* tree = buffer->tree;

    double sum;
    double min;
    double max;
    unsigned long long count;

    tree->deserialize();

    if(args->arg_count == 1)
    {
        char s[100];
        strncpy( s, args->args[0], args->lengths[0] );
        s[args->lengths[0]] = '\0';
        if(!tree->query(s,&sum,&count,&min,&max))
        {
            return NULL;
        }
    }
    else
    {
        vector<string> v;
        char s[100];
        for(int i = 0 ; i < args->arg_count; i++)
        {
            char s[100];
            strncpy( s, args->args[i], args->lengths[i]);
            s[args->lengths[i]] = '\0';
            v.push_back(s);
        }
        if(!tree->query(v,&sum,&count,&min,&max))
        {
            return NULL;
        }
    }

    sprintf(buffer->aggValues,"SUM = %f Count = %lld Min = %f Max = %f \0",sum,count,min,max);
    return buffer->aggValues;
}

#endif

