
#ifndef QCTREEITEM_H
#define QCTREEITEM_H

#include "Cell.h"

class QCTreeItem
{
public:

    Cell* upperBound;
    Cell* lowerBound;

    int id;
    int child;

    /** Default constructor */
    QCTreeItem();
    /** Default destructor */
    virtual ~QCTreeItem();
    /** Copy constructor
     *  \param other Object to copy from
     */
    QCTreeItem(const QCTreeItem& other);

    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    QCTreeItem& operator=(const QCTreeItem& other);


    bool operator<( const QCTreeItem& val ) const ;

    bool operator<( const QCTreeItem* val ) const ;
    void print();

protected:
private:
};

#endif // QCTREEITEM_H
