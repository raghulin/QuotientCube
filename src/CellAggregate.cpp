#include "CellAggregate.h"

CellAggregate::CellAggregate()
{
    sum = 0;
    count = 0;
    //min = 0;
    //max = 0;
    //ctor
    m_isValid = false;
}

CellAggregate::~CellAggregate()
{
    //dtor
}

CellAggregate::CellAggregate(const CellAggregate& other)
{
    sum	 = other.sum;
    count = other.count;
    min = other.min;
    max = other.max;
    m_isValid = true;
}

CellAggregate& CellAggregate::operator=(const CellAggregate& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    sum	 = rhs.sum;
    count = rhs.count;
    min = rhs.min;
    max = rhs.max;
    //m_isValid = rhs.isValid();

    return *this;
}


CellAggregate& CellAggregate::operator+=(const CellAggregate& rhs)
{

    sum += rhs.sum;
    count += rhs.count;

    if(min > rhs.min)
        min = rhs.min;

    if(max < rhs.max)
        max = rhs.max;

    m_isValid = true;

    return *this;
}

CellAggregate& CellAggregate::operator+=(const double& val)
{

    sum += val;
    count++;

    if(min > val)
        min = val;

    if(max < val)
        max = val;

    m_isValid = true;

    return *this;
}



void CellAggregate::operator+(const double& val)
{
    if(!m_isValid)
    {
        min = val;
        max = val;
    }
    else
    {
        if(min > val)
            min = val;

        if(max < val)
            max = val;
    }

    sum	 += val;
    count++;

    m_isValid = true;
}

bool CellAggregate::operator!=( const CellAggregate& val ) const
{

    if(sum != val.sum || count != val.count || min != val.min || max != val.max)
    {
        return false;
    }

    return true;
}

bool CellAggregate::isValid()
{
    return m_isValid;
}




