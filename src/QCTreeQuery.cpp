#include "QCTreeQuery.h"
#include "QCTreeNode.h"
#include "QCTreeQuery.h"



QCTreeQuery::QCTreeQuery()
{
    //ctor
}

QCTreeQuery::~QCTreeQuery()
{
    //dtor
}


bool QCTreeQuery::pointQuery(QCTreeNode* root,Cell* cell,CellAggregate* aggregate)
{

    if(root == NULL)
    {
        return false;
    }


    /*
    1. Initialize newRoot. newRoot = the root node of T
    */

    QCTreeNode* newRoot = root;

    /*
    2. for each value vi in q && newRoot != NULL
    // reach for the next node with label vi
    newRoot = searchRoute(newRoot, vi );
    */

    for(unsigned int i=0; i<cell->columns.size(); i++)
    {

        if(strcmp(cell->valueAt(i).c_str(),"*")==0)
        {
            continue;
        }

        QCTreeNode* tempNode = newRoot->searchRoute(cell->valueAt(i));

        if(tempNode != NULL /*&& tempNode->dimension == i*/)
        {
            newRoot = tempNode;
        }
        else
        {
            /*
            3. if newRoot = NULL, return null;
            */
            return false;
        }
    }

    if(newRoot != NULL)
    {
        /*
        else if it has aggregate value(s)
            return its aggregate(s);
        */
        if(newRoot->aggregate.isValid())
        {
            *aggregate = newRoot->aggregate;
            return true;
        }
        else
        {

            while(newRoot != NULL)
            {
                /*
                    else
                    Keep picking the child corresponding to the last dimension of the
                    current node, until we reach a node with aggregate
                    values, and return them;

                */

                if(newRoot->aggregate.isValid())
                {
                    break;
                }

                newRoot = newRoot->findLastChildInLastDimension();
            }

            if(newRoot != NULL)
            {
                *aggregate = newRoot->aggregate;
                return true;
            }

        }
    }
    else
    {
        return false;
    }

    return false;
}

bool QCTreeQuery::rangeQuery(QCTreeNode* root,Cell* cell,CellAggregate* aggregate)
{
    /*
    1. Let newRoot be the root node of T , results be Ø;
    2. results = rangeQuery(q, newRoot, 0);
    3. return results;
    */

    return rangeQuery(root,cell,aggregate,0);

}

bool QCTreeQuery::rangeQuery(QCTreeNode* root,Cell* query,CellAggregate* aggregate,int i)
{
    /*
    Function rangeQuery(q, newRoot, i)
    // base case;
    if i > the last non-* dimension in q,
        if newRoot = NULL. do nothing;
        if newRoot has aggregate value
            Add its aggregate to results
        else
            Keep picking the child with a value on the last dimension
            until reach a node with aggregate value, add its aggregate to results.
        return;
    //recursion;
    */
    QCTreeNode* newRoot = root;

    if(i > query->lastNonStarDimension())
    {
        if(newRoot == NULL)
        {

        }
        else if(newRoot->aggregate.isValid())
        {
            *aggregate += newRoot->aggregate;
        }
        else
        {
            while(newRoot != NULL)
            {
                newRoot = newRoot->findLastChildInLastDimension();
                if(newRoot != NULL && newRoot->aggregate.isValid())
                {
                    *aggregate += newRoot->aggregate;
                    break;
                }

            }
        }

        return true;
    }

    /*
    if in q, i is not a range dimension
        Call searchRoute(newRoot, vi ),
        Let newRoot be the return node
        if newRoot is not NULL
            Call rangeQuery(q, newRoot, i + 1)
        else, for each value vim in the range
            Call searchRoute(newRoot, vim ),
            Let newRoot be the return node
            if newRoot is not NULL
                Call rangeQuery(q, newRoot, i + 1)
    */
    /*
    const char* cindex = index(query->valueAt(i).c_str(),' ');

    if(cindex != NULL)
    {
        newRoot = newRoot->searchRoute(query->valueAt((i)));

        if(newRoot != NULL)
        {
            rangeQuery(newRoot,query,aggregate,i+1);
        }
    }
    else */
    {
        Cell c;
        c.setDimensions(query->valueAt(i),' ');
        for(unsigned int j=0; j<c.columns.size(); j++)
        {

            QCTreeNode* tempNode = newRoot->searchRoute(c.valueAt(j));

            if(tempNode != NULL)
            {
                //newRoot = tempNode;
                rangeQuery(tempNode,query,aggregate,i+1);
            }
        }
    }

    return true;
}
