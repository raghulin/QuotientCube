/*
  returns the cube of the values in a distribution

  input parameters:
	select ccube(a,b,c,s) from Q4;

  output:
	select ccube_query("*");

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
    char* output;
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
    for(int i = 0 ; i < (buffer->dimensions + buffer->measures); i++)
    {
        char* s = (char*) malloc(args->lengths[i]);
        //strncpy(s,args->args[i], args->lengths[i] );
        memcpy(s,args->args[i], args->lengths[i]);
        s[args->lengths[i]] = '\0';
        v.push_back(s);
    }
    buffer->tree->baseTable->addRow(v);
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
    initid->max_length	= 32;
    initid->ptr = (char*)buffer;

    return 0;
}

void ccube_query_deinit( UDF_INIT* initid )
{
    ccube_data *buffer = (ccube_data*)initid->ptr;

    delete buffer->output;
    delete buffer->tree->qctree;
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
        tree->query(s,&sum,&count,&min,&max);
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
        tree->query(v,&sum,&count,&min,&max);
    }

    buffer->output = (char*) malloc(80);
    bzero(buffer->output,80);
    int len = sprintf(buffer->output,"SUM = %f Count = %lld Min = %f Max = %f",sum,count,min,max);
    buffer->output[len] = '\n';
    buffer->output = (char *)realloc(buffer->output,len);
    return buffer->output;
}

#endif

