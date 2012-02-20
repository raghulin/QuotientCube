#ifndef CUBETABLE_H
#define CUBETABLE_H

#include "IndexSearch.h"
#include "CellAggregate.h"
#include <iostream>
#include <vector>

class CubeTableRow 
{
    
public:
    
    vector<string> dimensions;
    vector<double> measures;
    
    CubeTableRow& operator=(const CubeTableRow& rhs)
    {
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        
        dimensions = rhs.dimensions;
        measures = rhs.measures;
        
        return *this;
    }
};

class CubeTable
{

    vector<string> m_dimensions;
    vector<string> m_measures;

public:

    vector< vector <string> > indexes;
    vector < CubeTableRow > table;

    /** Default constructor */
    CubeTable();

    CubeTable(const CubeTable& other);

    CubeTable& operator=(const CubeTable& rhs);
    CubeTable(CubeTable& other,string value,int index);

    /** Default destructor */
    virtual ~CubeTable();

    void getAllUniqueValues(int coloumn,vector<string> *values);

    void computeAggregate(vector<string>& cols,CellAggregate* aggregateValue,
                          vector<string>& dcols,CellAggregate* daggregateValue);

    void computeIndexes();

    int dimensionCount();

    int rowCount();

    void addCol(string col);
    void addMeasure(string col);
    void addRow(CubeTableRow& row);
    void addRow(string row,vector<double> m);
    void addRow(string row,double m);

protected:


private:
};

#endif // CUBETABLE_H

