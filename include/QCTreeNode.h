#ifndef QCTREENODE_H
#define QCTREENODE_H

#include "Cell.h"

#include <map>

#include <iostream>
#include <fstream>
using namespace std;

struct eqstr1
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};

class QCTreeNode
{
public:
    
    hash_map<const char*, QCTreeNode*, hash<const char*>, eqstr1>  children;
    hash_map<const char*, QCTreeNode*, hash<const char*>, eqstr1>  drillDowns;
    
    string value;

    int dimension;

    CellAggregate aggregate;
    
    /** Default constructor */
    QCTreeNode();
    /** Default destructor */
    virtual ~QCTreeNode();
    /** Copy constructor
     *  \param other Object to copy from
     */
    QCTreeNode(const QCTreeNode& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    QCTreeNode& operator=(const QCTreeNode& other);

    bool insertNodes(Cell* cell,QCTreeNode** newNode);

    void addDrillDown(string label,QCTreeNode* node,Cell*);

    QCTreeNode* findChild(string key);
    QCTreeNode* findLastChildInLastDimension();
    QCTreeNode* findChild();

    QCTreeNode* findDrilldown(string key);

    QCTreeNode* findNode(Cell* cell1,int index);

    QCTreeNode* searchRoute(string vi);

    void print();
    void printToStream(fstream *fout);

    unsigned int nodeId;
};

#endif // QCTREENODE_H
