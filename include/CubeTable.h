//
// 
// https://github.com/Imaginea/imaginea.github.com
//
// Created by RaghuL on 21/12/2011.
// Copyright Imaginea 2011. All rights reserved.
//

#ifndef CUBETABLE_H
#define CUBETABLE_H

#include "IndexSearch.h"
#include "CellAggregate.h"
#include <iostream>
#include <vector>

class CubeTable
{

        vector<string> m_dimensions;
        vector<string> m_measures;

    public:

        vector< vector <string> > indexes;
        vector < vector <string> > table;

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
        void addRow(vector<string>& row);
        void addRow(string row);

    protected:


    private:
};

#endif // CUBETABLE_H

