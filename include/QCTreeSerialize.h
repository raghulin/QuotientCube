#ifndef QCTREESE_H
#define QCTREESE_H

#include "QCTreeItem.h"
#include "QCTreeNode.h"
#include "QCTree.h"

class QCTreeSerialize
{
public:

    /** Default constructor */
    QCTreeSerialize();
    /** Default destructor */
    virtual ~QCTreeSerialize();
    /** Copy constructor
     *  \param other Object to copy from
     */
    QCTreeSerialize(const QCTreeSerialize& other);

    virtual void serialize(QCTree* tree, ostream *sbuff);
    virtual void serializeChildren(QCTreeNode* node, ostream *sbuff);
    virtual void serializeDrilldowns(QCTreeNode* node, ostream *sbuff);

};

#endif // QCTREE_H
