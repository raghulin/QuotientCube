#include "QCTreeSerialize.h"
#include "QCTree.h"
#include "QCTreeQuery.h"
#include <iostream>
#include <fstream>
using namespace std;

QCTreeSerialize::QCTreeSerialize()
{
    //ctor
}

QCTreeSerialize::~QCTreeSerialize()
{
    //dtor

}

QCTreeSerialize::QCTreeSerialize(const QCTreeSerialize& other)
{
    //copy ctor
}

void QCTreeSerialize::serialize(QCTree* tree, ostream *sbuff)
{
    serializeChildren(tree->qctree, sbuff);
    serializeDrilldowns(tree->qctree, sbuff);
}

void QCTreeSerialize::serializeChildren(QCTreeNode* node, ostream *sbuff)
{
    *sbuff<<node->nodeId<<" ";
    *sbuff<<node->value<<" ";
    *sbuff<<node->dimension<<" ";
    *sbuff<<node->aggregate;
    *sbuff<<node->children.size()<<endl;
    
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;
    
    for(nodeIter = node->children.begin(); nodeIter != node->children.end( ); nodeIter++)
    {
        QCTreeNode* node1 = (QCTreeNode *) nodeIter->second;
        serializeChildren(node1, sbuff);
    }
}

void QCTreeSerialize::serializeDrilldowns(QCTreeNode* node, ostream *sbuff)
{
    *sbuff<<node->drillDowns.size();
    
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;
    
    for(nodeIter = node->drillDowns.begin(); nodeIter != node->drillDowns.end( ); nodeIter++)
    {
        QCTreeNode* node1 = (QCTreeNode *) nodeIter->second;
        *sbuff <<" "<< (nodeIter->first) ;
        *sbuff << " "<<node1->nodeId;
    }
    
    *sbuff << endl;
    
    for(nodeIter = node->children.begin(); nodeIter != node->children.end( ); nodeIter++)
    {
        QCTreeNode* node1 = (QCTreeNode *) nodeIter->second;
        serializeDrilldowns(node1, sbuff);
    }
}
