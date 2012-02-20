#include "CubeTable.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

CubeTable::CubeTable()
{

}

CubeTable::~CubeTable()
{
    indexes.erase(indexes.begin(),indexes.end());
    m_dimensions.erase(m_dimensions.begin(),m_dimensions.end());
    m_measures.erase(m_measures.begin(),m_measures.end());
    table.erase(table.begin(),table.end());
}

CubeTable::CubeTable(const CubeTable& other)
{
    m_dimensions = other.m_dimensions;
    table = other.table;
    m_measures = other.m_measures;
}

CubeTable::CubeTable(CubeTable& other,string value,int index)
{
    m_dimensions = other.m_dimensions;
    vector<string> values[m_dimensions.size()];

    vector< CubeTableRow >::iterator iter_ii;

    for(iter_ii=other.table.begin(); iter_ii!=other.table.end(); iter_ii++)
    {
        string s = (*iter_ii).dimensions[index];

        if(strcmp(value.c_str(),s.c_str()) == 0)
        {
            for(int j=0; j<m_dimensions.size(); j++)
            {
                string s = (*iter_ii).dimensions[j];
                values[j].push_back(s);
            }

            table.push_back(*iter_ii);
        }
    }

    m_measures = other.m_measures;

    for(int j=0; j<m_dimensions.size(); j++)
    {
        std::sort( values[j].begin(), values[j].end());
        vector<string>::iterator new_end_pos;
        new_end_pos = std::unique( values[j].begin(), values[j].end() );
        values[j].erase( new_end_pos, values[j].end() );

        indexes.push_back(values[j]);
    }
}


CubeTable& CubeTable::operator=(const CubeTable& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator

    m_dimensions = rhs.m_dimensions;
    indexes = rhs.indexes;

    table = rhs.table;
    m_measures = rhs.m_measures;

    return *this;
}

void CubeTable::addCol(string col)
{
    m_dimensions.push_back(col);
}
void CubeTable::addMeasure(string col)
{
    m_measures.push_back(col);
}
void CubeTable::addRow(CubeTableRow &row)
{
    table.push_back(row);
}

void CubeTable::addRow(string row,vector<double> m)
{
    using namespace std;

    CubeTableRow trow;
    vector<string> tokens;
    istringstream iss(row);

    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter< vector<string> >(tokens)
         );

    trow.dimensions = tokens;
    trow.measures = m;
    addRow(trow);
}

void CubeTable::addRow(string row,double m)
{
		vector<double> md;
		md.push_back(m);

		addRow(row,md);
}

void CubeTable::getAllUniqueValues(int coloumn,vector<string> *values)
{
    vector< CubeTableRow >::iterator iter_ii;

    for(iter_ii=table.begin(); iter_ii!=table.end(); iter_ii++)
    {
        values->push_back((*iter_ii).dimensions[coloumn]);
    }

    std::sort( values->begin(), values->end());

    vector<string>::iterator new_end_pos;
    new_end_pos = std::unique( values->begin(), values->end() );
    values->erase( new_end_pos, values->end() );
}


void CubeTable::computeIndexes()
{
    indexes.clear();
    for(int i = 0 ; i< dimensionCount(); i++)
    {
        vector<string> values;
        getAllUniqueValues(i,&values);
        indexes.push_back(values);
    }
}

void CubeTable::computeAggregate(vector<string>& cols,CellAggregate* aggregateValue,
                                 vector<string>& dcols,CellAggregate* daggregateValue)
{
    unsigned int cz = cols.size();

    vector< CubeTableRow >::iterator iter_ii;
    for(iter_ii=table.begin(); iter_ii!=table.end(); iter_ii++)
    {
        for(unsigned int i =0 ; i< (m_measures.size()); i++)
        {

            unsigned int j = 0;

            for(; j< cz; j++)
            {
                if(strcmp(cols[j].c_str(),"*") != 0 && strcmp(cols[j].c_str(),(*iter_ii).dimensions[j].c_str()) != 0)
                {
                    break;
                }
            }

            if(j ==  cz)
            {
                *aggregateValue += (*iter_ii).measures[i];
            }

            j = 0;

            for(; j< cz; j++)
            {
                if(strcmp(dcols[j].c_str(),"*") != 0 && strcmp(dcols[j].c_str(),(*iter_ii).dimensions[j].c_str()) != 0)
                {
                    break;
                }
            }

            if(j ==  cz)
            {
                *daggregateValue += (*iter_ii).measures[i];
            }
        }
    }
}

int CubeTable::rowCount()
{
    return table.size();
}


int CubeTable::dimensionCount()
{
    return m_dimensions.size();
}
