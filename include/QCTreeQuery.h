#ifndef QCTREEQUERY_H
#define QCTREEQUERY_H

#include "QCTreeNode.h"
#include "CellAggregate.h"

class QCTreeQuery
{
public:
    /** Default constructor */
    QCTreeQuery();
    /** Default destructor */
    virtual ~QCTreeQuery();

    bool pointQuery(QCTreeNode* root,Cell* cell,CellAggregate* aggregate);

    bool rangeQuery(QCTreeNode* root,Cell* cell,CellAggregate* aggregate);
    bool rangeQuery(QCTreeNode* root,Cell* query,CellAggregate* aggregate,int i);


protected:
private:
};

#endif // QCTREEQUERY_H
