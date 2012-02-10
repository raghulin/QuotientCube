#ifndef INDEXSEARCH_H
#define INDEXSEARCH_H

#include <string.h>
#include <ext/hash_map>

using namespace std;

using namespace __gnu_cxx;

struct eqstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};

class IndexSearch
{
public:

    hash_map<const char*, int, hash<const char*>, eqstr>  m_indexHash;

    /** Default constructor */
    IndexSearch();
    /** Default destructor */
    virtual ~IndexSearch();

    void addIndex(string key,int value);

    int keyValue(string key);

    const char* firstKey();


protected:


private:
};

#endif // INDEXSEARCH_H
