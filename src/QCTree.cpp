#include "QCTree.h"
#include "QCTreeQuery.h"
#include <iostream>
#include <fstream>
using namespace std;

QCTree::QCTree()
{
    //ctor
    m_currentClassId = 0;
    m_numberOfClasses = 0;
    qctree = new QCTreeNode();
    baseTable = new CubeTable();
    root = NULL;
}

QCTree::~QCTree()
{
    //dtor

    delete qctree;
    delete baseTable;
    if(root)
    {
        delete root;
    }

    classes.erase(classes.begin(),classes.end());
}

QCTree::QCTree(const QCTree& other)
{
    //copy ctor
}

QCTree& QCTree::operator=(const QCTree& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}


void QCTree::printTempClasses()
{
    vector <QCTreeItem*> :: const_iterator nodeIter;

    for(nodeIter = classes.begin(); nodeIter != classes.end(); nodeIter++)
    {
        QCTreeItem* node = *nodeIter;
        node->print();
    }

}

struct compareTreeItems
{
    const bool operator()(const QCTreeItem *a, const QCTreeItem * b) const
    {
        /*
        a->upperBound->print();
        cout<< (*a->upperBound < *b->upperBound);
        b->upperBound->print();
        cout<<endl;
        */
        return (*b->upperBound < *a->upperBound);
    }
};


void QCTree::sortIntermediate()
{

    std::sort(classes.begin(),classes.end(),compareTreeItems());

}

bool QCTree::isAllTempClassesCovered()
{

    if(m_currentClassId < m_numberOfClasses)
    {
        return false;
    }

    return true;

}

QCTreeItem* QCTree::getClass(int id)
{

    vector <QCTreeItem*> :: const_iterator nodeIter;

    for(nodeIter = classes.begin(); nodeIter != classes.end(); nodeIter++)
    {
        QCTreeItem* node = *nodeIter;

        if(node->id == id)
        {
            return node;
        }
    }

    if(id == -1)
    {
        return classes[0];
    }

    return NULL;
}


QCTreeItem* QCTree::getNextClass()
{
    m_currentClassId++;
    if(m_currentClassId < m_numberOfClasses)
    {
        QCTreeItem* qitem = (QCTreeItem *)classes[m_currentClassId];
        return qitem;//->upperBound;
    }
    else
    {
        return NULL;
    }
}

void QCTree::constructTree()
{

    baseTable->computeIndexes();

    /*
    	1. b = (∗, . . . , ∗);
    */
    Cell* cell = new Cell(baseTable->dimensionCount());
    /*
    	2. call DF S(b, B, 0, −1);
    */
    DFS(cell,baseTable,0,-1);
    /*
    	3. Sort the temp classes w.r.t. upper bounds in dictionary order(‘‘∗’’ precedes other values)
    */

    sortIntermediate();

    m_currentClassId = 0;
    m_numberOfClasses = classes.size();


    //Insert the temp classes one by one into QC-tree
    /*
    	4. Create a node for the first temp class as the root;
    */
    root = classes[0]->upperBound;
    root->aggregate = classes[0]->upperBound->aggregate;
    qctree->aggregate = root->aggregate;
    /*
    	5. last = first temp class’s upper bound;
    */

    Cell* lastClass = classes[0]->upperBound;

    /*
    	6. while not all temp classes have been processed
    */
    QCTreeItem* nextClass = classes[m_currentClassId];//->upperBound;

    QCTreeNode* lastInserted =qctree;
    /*not all temp classes have been processed*/
    while(!isAllTempClassesCovered())
    {
        /*
        current = next class’s upper bound;
        */

        Cell* currentClass = nextClass->upperBound;

        if(*currentClass != *lastClass)
        {
            /*
            insert nodes for current;
            last = current;
            */
            qctree->insertNodes(currentClass,&lastInserted);
            lastClass = currentClass;
        }
        else
        {
            /*
            Let ub be the current’s child class’s upper bound,
            lb be the lower bound of current;
            */

            QCTreeItem* currentsChild = getClass(nextClass->child);

            Cell* ub = currentsChild->upperBound;
            Cell* lb = nextClass->lowerBound;

            /*
            Find the first dim D s.t.ub.D = ∗ && lb.D != ∗;
            Add a drill-down link with label D from ub to last;
            */
            for(int j=0; j<baseTable->dimensionCount(); j++)
            {
                if(strcmp(ub->valueAt(j).c_str(),"*") == 0 &&
                        strcmp(lb->valueAt(j).c_str(),"*") != 0
                  )
                {
                    qctree->addDrillDown(lb->valueAt(j),lastInserted,ub);
                }
            }
        }
        nextClass = getNextClass();
    }

    delete cell;
}


//c is a cell and Bc is the partition of the base table
void QCTree::DFS(Cell* c,CubeTable* partition,int k,int pID)
{

    /*
    2. Compute the upper bound d of the class c, by ‘‘jumping’’ to the appropriate upper bounds;
    */
    Cell* d = c->upperBound(partition);

    /*
    1. Compute aggregate of cell c;
    */
    partition->computeAggregate(c->columns,&c->aggregate,d->columns,&d->aggregate);

    /*
    3. Record a temp class with lower bound c, upper bound d and and parent pID. Let bID be its class ID;
    */

    QCTreeItem* item = new QCTreeItem();;
    item->lowerBound = new Cell(*c);
    item->upperBound = new Cell(*d);
    item->child = pID;
    item->id = m_numberOfClasses;
    int bID = item->id;

    classes.push_back(item);
    m_numberOfClasses++;

    /*
    4. if there is some j < k s.t. c[j] = all and d[j] != all
    return; //such a bound has been examined before
    */
    for(int j=0; j<k; j++)
    {
        if(strcmp(c->valueAt(j).c_str(),"*") == 0
                && strcmp(d->valueAt(j).c_str(),"*") != 0
          )
        {
            return;
        }
    }


    /*
    5. else for each k < j < n s.t. d[j] = all do
    		for each value x in dimension j of base table
    			let d[j] = x;
    			form Bd ;
    			if Bd is not empty, call DF S(d, Bd , j, bID);
    */


    for(int j=k; j<partition->dimensionCount(); j++)
    {
        if(strcmp(d->valueAt(j).c_str(),"*") ==0)
        {
            vector<string> x;

            partition->getAllUniqueValues(j,&x);
            //std::reverse( x.begin(), x.end());

            Cell *nd = new Cell(*d);
            for(int rows = x.size()-1; rows >= 0; rows--)
            {
                nd->setValueAt(j,x[rows]);
                //form Bd - ?
                CubeTable* Bd = new CubeTable(*partition,x[rows],j);

                if(Bd->table.size() != 0)
                {
                    DFS(nd,Bd,j,bID);
                }
                delete Bd;
            }
            delete nd;

        }
    }

    /*
    6. return;
    */
}

void QCTree::serialize()
{
    fstream filestr;

    //TODO:
    // 1. get table name from UDF. Serialize with table name.
    // 2. store in memory table

    filestr.open ("qctree.txt", fstream::out );

    qctree->serialize(&filestr);

    filestr.close();

}

void QCTree::deserialize()
{
    fstream filestr1;

    //TODO:
    // 1. get table name from UDF. Serialize with table name.
    // 2. read from in memory table

    filestr1.open ("qctree.txt", fstream::in );

    qctree->deserialize(&filestr1);

    filestr1.close();
}

bool QCTree::query(const char* s,double *agg,	unsigned long long  *count,double* min,double* max)
{
    QCTreeQuery query;
    Cell cell;
    cell.setDimensions(s,',');
    CellAggregate cagg;
    bool retVal = query.pointQuery(qctree,&cell,&cagg);

    *agg = cagg.sum;
    *count = cagg.count;
    *min = cagg.min;
    *max = cagg.max;

    return retVal;
}

bool QCTree::query(vector<string> s,double *agg,unsigned long long  *count,double* min,double* max)
{
    QCTreeQuery query;
    Cell cell;
    cell.setDimensions(s);
    CellAggregate cagg;
    bool retVal = query.rangeQuery(qctree,&cell,&cagg);

    *agg = cagg.sum;
    *count = cagg.count;
    *min = cagg.min;
    *max = cagg.max;

    return retVal;
}

void QCTree::printTree()
{
    qctree->print();
}
