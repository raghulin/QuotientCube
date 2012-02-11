#ifndef QCTREE_H
#define QCTREE_H

#include "QCTreeItem.h"
#include "QCTreeNode.h"

class QCTree
{
public:

    /** Default constructor */
    QCTree();
    /** Default destructor */
    virtual ~QCTree();
    /** Copy constructor
     *  \param other Object to copy from
     */
    QCTree(const QCTree& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    QCTree& operator=(const QCTree& other);

    CubeTable* baseTable;

    Cell* root;

    vector<QCTreeItem*> classes;

    void constructTree();
    void DFS(Cell* c,CubeTable* partition,int k,int pID);
    void sortIntermediate();

    void serialize();
    void deserialize();


    void printTree();
    void printTempClasses();

    QCTreeNode* qctree;

    bool query(const char* s,double *agg,	unsigned long long  *count,double* min,double* max);
    bool query(vector<string> s,double *agg,	unsigned long long  *count,double* min,double* max);

protected:

    bool isAllTempClassesCovered();

    QCTreeItem* getNextClass();
    QCTreeItem* getClass(int id);


private:

    int m_currentClassId;
    int m_numberOfClasses;

};

#endif // QCTREE_H
