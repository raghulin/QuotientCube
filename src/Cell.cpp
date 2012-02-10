#include "Cell.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


Cell::Cell()
{
    //ctor
}

Cell::~Cell()
{
    //dtor
    columns.erase(columns.begin(),columns.end());
}

Cell::Cell(const Cell& other)
{
    cols = other.cols;
    Cell *c1 = (Cell *)&other;
    for(int i =0; i<cols; i++)
    {
        string s;
        s = c1->valueAt(i);
        columns.push_back(s);
    }

    aggregate	 = c1->aggregate;
}

Cell& Cell::operator=(const Cell& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

bool Cell::operator!=( const Cell& val ) const
{

    if(columns == val.columns)
    {
        return false;
    }

    return true;
}


Cell::Cell(int cols1)
{
    cols = cols1;

    for(int i =0; i<cols; i++)
    {
        columns.push_back("*");
    }

}

void Cell::setDimensions(vector<string> d)
{
    cols = d.size();
    for(unsigned int c = 0; c < d.size(); c++)
    {
        columns.push_back(d[c]);
    }
}

void Cell::setDimensions(string value,char delim)
{
    using namespace std;

    vector<string> tokens;
    istringstream iss(value);

    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));

    setDimensions(tokens);
}

string Cell::valueAt(int pos)
{
    return columns[pos];
}


void Cell::setValueAt(int pos,string val)
{
    columns[pos] = val;
}

int Cell::lastNonStarDimension()
{
    for(int c = columns.size()-1; c >= 0 ; c--)
    {
        if(strcmp(valueAt(c).c_str(),"*") != 0)
        {
            return c;
        }
    }

    return 0;
}

/*
    #
    # the upper bound is the same as the lower bound for every
    # value of the lower bound that isn't '*'
    #
    # for any value of the lower bound that is '*' if there
    #  is a single value that appears in every tuple of the
    #  partition that we're looking over, then the value
    #  of the upper bound for that dimension is the value
    #  that appears in every tuple
    #
    # This is the 'jumping' described in the literature
    #
    # Example: if our partition is:
    # (2, 1, 1), (2, 1, 2)
    #
    # and our lower bound is
    # (2, *, *)
    #
    # our upper bound is
    # (2, 1, *)
    #
*/

Cell* Cell::upperBound(CubeTable* table)
{
    Cell* upper = new Cell();

    upper->setDimensions(columns);

    for(unsigned int c = 0; c < columns.size(); c++ )
    {
        if(strcmp(columns[c].c_str(),"*") ==0)
        {
            vector<string> indexVectors = table->indexes[c];

            if(table->indexes[c].size() == 1)
            {
                upper->columns[c] = indexVectors[0];
            }
        }
        else
        {
            upper->columns[c] = columns[c];
        }
    }

    return upper;

}

Cell* Cell::lowerBound(CubeTable* table)
{

    Cell* lower = new Cell(*this);

    return lower;
}

bool Cell::operator<( const Cell& val ) const
{

    string star = "*";

    Cell* c = (Cell* )&val;
    Cell* c1 = (Cell* ) this;

    for(unsigned int i=0; i<columns.size(); i++)
    {

        if(strcmp(c1->valueAt(i).c_str(), star.c_str()) == 0
                && strcmp(c->valueAt(i).c_str(), star.c_str()) != 0)
        {
            return false;
        }

        if(strcmp(c->valueAt(i).c_str(), star.c_str()) == 0
                && strcmp(c1->valueAt(i).c_str(), star.c_str()) != 0)
        {
            return true;
        }

        if(strcmp(c1->valueAt(i).c_str(), c->valueAt(i).c_str()) == 0)
        {
            continue;
        }
        else if(strcmp(c1->valueAt(i).c_str(),c->valueAt(i).c_str()) > 0)
        {

            int position = strcmp(c->valueAt(i).c_str(),c1->valueAt(i).c_str());
            char cc = c->valueAt(i).c_str()[position];
            char cc1 = c1->valueAt(i).c_str()[position];

            if((tolower(cc1) - tolower(cc)) > 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

void Cell::print()
{

    cout<< " <";
    unsigned int i=0;
    for(; i < columns.size()-1; i++)
    {
        cout<<columns[i] <<",";
    }

    cout<<columns[i] <<"> ";
}

string Cell::toString()
{
    string s = "<";
    unsigned int i=0;
    for(; i < columns.size()-1; i++)
    {
        s+= columns[i] + ",";
    }

    s+= columns[i] +">";

    return s;
}
