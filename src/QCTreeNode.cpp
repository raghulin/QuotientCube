#include "QCTreeNode.h"
#include <iostream>
#include <fstream>

using namespace std;

static unsigned int staticNodeIncrement;

QCTreeNode::QCTreeNode()
{
    value = "*";
    nodeId = staticNodeIncrement++;
}

QCTreeNode::~QCTreeNode()
{
    drillDowns.erase(drillDowns.begin(),drillDowns.end());

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator hm1_RcIter;

    for(hm1_RcIter = children.begin(); hm1_RcIter != children.end( ); hm1_RcIter++)
    {
        QCTreeNode* node = (QCTreeNode *) hm1_RcIter->second;
        if(node)
        {
            delete node;
        }
    }
}

QCTreeNode::QCTreeNode(const QCTreeNode& other)
{
    //copy ctor
}

QCTreeNode& QCTreeNode::operator=(const QCTreeNode& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

QCTreeNode* QCTreeNode::findDrilldown(string key)
{

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator hm1_RcIter;

    hm1_RcIter = drillDowns.find(key.c_str());


    if ( hm1_RcIter == drillDowns.end( ) )
    {
        return NULL;
    }
    else
    {
        return hm1_RcIter->second;
    }
}

QCTreeNode* QCTreeNode::findLastChildInLastDimension()
{
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    QCTreeNode* node = NULL;

    nodeIter = children.begin();

    while(nodeIter != children.end())
    {
        QCTreeNode* tempNode = nodeIter->second;

        if(tempNode != NULL)
        {
            if(node == NULL)
            {
                node = tempNode;
            }
            else if(node->dimension <= tempNode->dimension)
            {
                node = tempNode;
            }
        }

        nodeIter++;
    }

    return node;
}



QCTreeNode* QCTreeNode::findChild(string key)
{
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;
    nodeIter = children.find(key.c_str());

    if ( nodeIter == children.end() )
    {
        return NULL;
    }
    else
    {
        return nodeIter->second;
    }
}

QCTreeNode* QCTreeNode::searchRoute(string vi)
{

    //find a route from newRoot to a node labelled vi
    QCTreeNode* newRoot = NULL;


    if(strcmp(vi.c_str(),"*")==0)
    {
        return this;
    }


    //if newRoot has a child or link pointing to N labelled vi
    //newRoot = N ;
    newRoot = findChild(vi);

    if(newRoot == NULL)
    {
        newRoot = findDrilldown(vi);
    }

    if(newRoot == NULL)
    {

        //Pick the last child N of newRoot in the last dimension, say j;
        //if (j < the dimension of vi ) call searchRoute(N, vi );
        //else return null;

        newRoot = findLastChildInLastDimension();

        if(newRoot!=NULL && dimension < newRoot->dimension)
        {
            newRoot = newRoot->searchRoute(vi);
        }
    }

    return newRoot;

}

bool QCTreeNode::insertNodes(Cell* cell1,QCTreeNode** newNode)
{
    bool isNodeInserted = false;

    Cell* cell = new Cell(*cell1);

    for(int i=0; i< cell->cols; i++)
    {
        if(strcmp(cell->valueAt(i).c_str(),"*") == 0)
        {
            continue;
        }

        QCTreeNode* node = findChild(cell->valueAt(i));

        if(node == NULL)
        {
            node = new QCTreeNode();
            node->value = cell->valueAt(i);
            node->dimension = i;
            cell->setValueAt(i,"*");
            *newNode = node;
            isNodeInserted = node->insertNodes(cell,newNode);
            if(!isNodeInserted)
            {
                node->aggregate = cell->aggregate;
                node->aggregate.setValid();
            }
            else
            {
                node->aggregate.setInvalid();
            }

            children.insert(pair<const char*,QCTreeNode*>(node->value.c_str(),node));

            isNodeInserted = true;
            break;
        }
        else
        {
            cell->setValueAt(i,"*");
            isNodeInserted = node->insertNodes(cell,newNode);
            break;
        }

    }

    delete cell;

    return isNodeInserted;
}

QCTreeNode* QCTreeNode::findNode(Cell* cell,int index)
{
    QCTreeNode* node = this;

    while(index < (cell->cols) && (strcmp(cell->valueAt(index).c_str(),"*") == 0))
    {
        index++;
    }

    if(index < (cell->cols))
    {
        QCTreeNode* tempNode = node->findChild(cell->valueAt(index));
        if(tempNode != NULL)
        {
            node = tempNode->findNode(cell,index+1);
        }

        if(node == NULL)
        {
            node = tempNode;
        }
    }

    return node;
}


void QCTreeNode::addDrillDown(string label,QCTreeNode* node,Cell* fromCell)
{

    if(node != NULL)
    {
        QCTreeNode* fromNode = findNode(fromCell,0);

        if(fromNode != NULL)
        {
            fromNode->drillDowns.insert(pair<const char*,QCTreeNode*>(label.c_str(),node));
        }
    }

}


void QCTreeNode::deserialize(istream *sbuff)
{
    map<unsigned int,QCTreeNode*> nodeMap;
    deserializeChildren(sbuff,&nodeMap);
    deserializeDrilldowns(sbuff,&nodeMap);
}

void QCTreeNode::deserializeChildren(istream *sbuff,map<unsigned int,QCTreeNode*> *nodeMap)
{
    int noOfChildren =0;

    *sbuff >>nodeId;
    *sbuff >>value;
    *sbuff >>dimension;
    *sbuff >>aggregate;
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
        newChild->deserializeChildren(sbuff,nodeMap);

        children.insert(pair<const char*,QCTreeNode*>(newChild->value.c_str(),newChild));
    }
}
void QCTreeNode::deserializeDrilldowns(istream *sbuff,map<unsigned int,QCTreeNode*> *nodeMap)
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

        drillDowns.insert(pair<const char*,QCTreeNode*>(label,temp));
    }

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        node->deserializeDrilldowns(sbuff,nodeMap);
    }

}

void QCTreeNode::serialize(ostream *sbuff)
{
    serializeChildren(sbuff);
    serializeDrilldowns(sbuff);
}

void QCTreeNode::serializeChildren(ostream *sbuff)
{
    *sbuff<<nodeId<<" ";
    *sbuff<<value<<" ";
    *sbuff<<dimension<<" ";
    *sbuff<<aggregate;
    *sbuff<<children.size()<<endl;

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        node->serializeChildren(sbuff);
    }
}

void QCTreeNode::serializeDrilldowns(ostream *sbuff)
{
    *sbuff<<drillDowns.size();

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    for(nodeIter = drillDowns.begin(); nodeIter != drillDowns.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        *sbuff <<" "<< (nodeIter->first) ;
        *sbuff << " "<<node->nodeId;
    }

    *sbuff << endl;

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        node->serializeDrilldowns(sbuff);
    }
}

void QCTreeNode::printToStream(fstream *pfout)
{

    *pfout << " -------------- TREE Node: " << value << " "<< aggregate <<endl;

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    *pfout << "TREE Node Children: ";

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        *pfout << " " <<node->value << " ";
    }

    *pfout <<endl<< "TREE Node Drilldowns: ";
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator dds;
    for(dds = drillDowns.begin(); dds != drillDowns.end(); dds++)
    {
        QCTreeNode* node = (QCTreeNode *) dds->second;
        *pfout << (dds->first)<<"--->"<<(node->value) << "     ";
    }
    *pfout << endl<<" -------------- TREE Node ---------------------- "<<endl;

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        node->printToStream(pfout);
    }
}

void QCTreeNode::print()
{

    cout << " -------------- TREE Node: " << value << " "<< aggregate <<endl;

    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator nodeIter;

    cout << "TREE Node Children: ";

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        cout << " " <<node->value << " ";
    }

    cout <<endl<< "TREE Node Drilldowns: ";
    hash_map <const char*, QCTreeNode*, hash<const char*>, eqstr1> :: const_iterator dds;
    for(dds = drillDowns.begin(); dds != drillDowns.end(); dds++)
    {
        QCTreeNode* node = (QCTreeNode *) dds->second;
        cout << (dds->first)<<"--->"<<(node->value) << "     ";
    }
    cout << endl<<" -------------- TREE Node ---------------------- "<<endl;

    for(nodeIter = children.begin(); nodeIter != children.end( ); nodeIter++)
    {
        QCTreeNode* node = (QCTreeNode *) nodeIter->second;
        node->print();
    }
}
