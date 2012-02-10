#include "QCTreeItem.h"

QCTreeItem::QCTreeItem()
{

    //ctor
}

QCTreeItem::~QCTreeItem()
{
    //dtor
    if(upperBound)
    {
        delete upperBound;
    }

    if(lowerBound)
    {
        delete lowerBound;
    }
}

QCTreeItem::QCTreeItem(const QCTreeItem& other)
{
    //copy ctor
}

QCTreeItem& QCTreeItem::operator=(const QCTreeItem& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    upperBound = rhs.upperBound;
    lowerBound = rhs.lowerBound;
    id = rhs.id;

    child = rhs.child;

    //assignment operator
    return *this;
}

bool QCTreeItem::operator<( const QCTreeItem* val ) const
{

    Cell c = *((Cell* ) val->upperBound);
    Cell c1 = *((Cell* ) upperBound);

    if(c1 < c)
    {
        return true;
    }

    return false;
}

bool QCTreeItem::operator<( const QCTreeItem& val ) const
{

    Cell c = *((Cell* ) val.upperBound);
    Cell c1 = *((Cell* ) upperBound);

    if(c1 < c)
    {
        return true;
    }

    return false;
}

void QCTreeItem::print()
{

    //cout << "Temporary Class: Id = " ;
    cout <<  id ;
    upperBound->print();
    lowerBound->print();
    cout<<" " << child << "  " <<upperBound->aggregate;
    cout <<endl;
}
