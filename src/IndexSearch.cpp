#include "IndexSearch.h"

IndexSearch::IndexSearch()
{
    //ctor
}

IndexSearch::~IndexSearch()
{
    m_indexHash.erase(m_indexHash.begin(),m_indexHash.end());
}

void IndexSearch::addIndex(string key,int value)
{
    m_indexHash.insert(pair<const char*,int>(key.c_str(),value));
}


const char* IndexSearch::firstKey()
{

    hash_map <const char*, int, hash<const char*>, eqstr> :: const_iterator key_Iter;

    key_Iter = m_indexHash.begin();

    return (*key_Iter).first;

}

int IndexSearch::keyValue(string key)
{

    hash_map <const char*, int, hash<const char*>, eqstr> :: const_iterator key_Iter;

    key_Iter = m_indexHash.find(key.c_str());

    if ( key_Iter == m_indexHash.end() )
    {
        return 0;
    }
    else
    {
        return key_Iter->second;
    }
}
