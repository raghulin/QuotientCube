#ifndef CELL_H
#define CELL_H

#include"IndexSearch.h"
#include"CubeTable.h"
#include"CellAggregate.h"

class Cell
{
public:

    CellAggregate aggregate;

    int id;
    int cols;

    /** Default constructor */
    Cell();
    /** Default destructor */
    virtual ~Cell();
    /** Copy constructor
     *  \param other Object to copy from
     */
    Cell(const Cell& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    Cell& operator=(const Cell& other);

    vector<string> columns;

    Cell(int cols);

    string valueAt(int pos);
    void setValueAt(int pos,string val);

    void setDimensions(vector<string> d);
    void setDimensions(string value,char delim);

    Cell* upperBound(CubeTable* table);
    Cell* lowerBound(CubeTable* table);

    bool operator<( const Cell& val ) const;

    bool operator!=( const Cell& val ) const;

    void print();

    string toString();

    int lastNonStarDimension();

protected:


private:
};

#endif // CELL_H
