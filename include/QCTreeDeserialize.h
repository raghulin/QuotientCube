#ifndef QCTREEDES_H
#define QCTREEDES_H

#include "QCTree.h"
#include "QCTreeItem.h"
#include "QCTreeNode.h"

class QCTreeDeserialize : QCTreeNode
{
public:

    /** Default constructor */
    QCTreeDeserialize();
    /** Default destructor */
    virtual ~QCTreeDeserialize();

    virtual void deserialize(QCTree* tree, istream *sbuff);
    virtual void deserializeChildren(QCTreeNode* node, istream *sbuff, map<unsigned int,QCTreeNode*> *nodeMap);
    virtual void deserializeDrilldowns(QCTreeNode* node, istream *sbuff, map<unsigned int,QCTreeNode*> *nodeMap);

};

#endif // QCTREE_H
