
#ifndef CELLAGG_H
#define CELLAGG_H

#include <iostream>

using namespace std;

class CellAggregate
{
public:

    //Aggregates
    double sum;
    long long count;
    double min;
    double max;

    /** Default constructor */
    CellAggregate();
    /** Default destructor */
    virtual ~CellAggregate();
    /** Copy constructor
     *  \param other Object to copy from
     */
    CellAggregate(const CellAggregate& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    CellAggregate& operator=(const CellAggregate& other);

    CellAggregate& operator+=(const CellAggregate& other);

    CellAggregate& operator+=(const double& other);

    void operator+(const double& val);

    bool operator!=( const CellAggregate& val ) const;

    friend ostream& operator<< (ostream &out, CellAggregate &agg)
    {
        out <<agg.m_isValid<<" "<<agg.sum <<" "<<agg.count <<" "<<agg.min <<" "<<agg.max <<" ";
    }
    friend istream& operator>> (istream &in, CellAggregate &agg)
    {
        in >> agg.m_isValid;
        in >> agg.sum;
        in >> agg.count;
        in >> agg.min;
        in >> agg.max;
    }

    void setInvalid()
    {
        m_isValid = false;
    }

    void setValid()
    {
        m_isValid = true;
    }


    bool isValid();

protected:
    bool m_isValid;

private:

};

#endif // CELLAGG_H
