#include "QCTreeDeserialize.h"
#include "QCTree.h"
#include "QCTreeQuery.h"
#include <iostream>
#include <fstream>
using namespace std;

QCTreeDeserialize::QCTreeDeserialize()
{
    //ctor
}

QCTreeDeserialize::~QCTreeDeserialize()
{
    //dtor

}

void QCTreeDeserialize::deserialize(QCTree* tree, istream *sbuff)
{
    map<unsigned int,QCTreeNode*> nodeMap;
    deserializeChildren(tree->qctree, sbuff,&nodeMap);
    deserializeDrilldowns(tree->qctree, sbuff,&nodeMap);
}

void QCTreeDeserialize::deserializeChildren(QCTreeNode* node, istream *sbuff,map<unsigned int,QCTreeNode*> *nodeMap)
{
    int noOfChildren =0;
    
    *sbuff >>node->nodeId;
    *sbuff >>node->value;
    *sbuff >>node->dimension;
    *sbuff >>node->aggregate;
    *sbuff >>noOfChildren;
    
    map <unsigned int, QCTreeNode*> :: const_iterator nodeIter;
    nodeIter = nodeMap->find(nodeId);
    
    if ( nodeIter == nodeMap->end() )
    {
        nodeMap->insert(pair<int,QCTreeNode*>(nodeId,this));
    }
    
    for(int i=0; i < noOfChildren; i++)
    {
        QCTreeNode *newChild = new QCTreeNode();
        deserializeChildren(newChild, sbuff,nodeMap);
        
        node->children.insert(pair<const char*,QCTreeNode*>(newChild->value.c_str(),newChild));
    }
}
void QCTreeDeserialize::deserializeDrilldowns(QCTreeNode* node, istream *sbuff,map<unsigned int,QCTreeNode*> *nodeMap)
{
    
    int noOfDrillDowns =0 ;
    *sbuff >>noOfDrillDowns;
    
    for(int i=0; i < noOfDrillDowns; i++)
    {
        string s;
        *sbuff >> s;
        unsigned int snID;
        *sbuff >> snID;
        
        map <unsigned int, QCTreeNode*> :: const_iterator nodeIter;
        nodeIter = nodeMap->find(snID);
        
        QCTreeNode* temp;
        
        if ( nodeIter != nodeMap->end() )
        {
            temp = nodeIter->second;
        }
        else
        {
            continue;
        }
        
        char* label = new char[sizeof(s)+1];
        
        strcpy(label,s.c_str());
        
        node->drillDowns.insert(pair<const char*,QCTreeNode*>(label,temp));
    }
    
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;
    
    for(nodeIter = node->children.begin(); nodeIter != node->children.end( ); nodeIter++)
    {
        QCTreeNode* node1 = (QCTreeNode *) nodeIter->second;
        deserializeDrilldowns(node1, sbuff,nodeMap);
    }
    
}