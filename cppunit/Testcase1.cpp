#include "Testcase1.h"

#include <iostream>
#include <QCTree.h>
#include <QCTreeQuery.h>
#include <Cell.h>

using namespace std;

#include<iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

CPPUNIT_TEST_SUITE_REGISTRATION( Testcase1 );

void Testcase1::testUppers(QCTree* tree,string outputUppers)
{


    vector<QCTreeItem*> tempClasses = tree->classes;

    vector <QCTreeItem*> :: const_iterator nodeIter;


    // Test the upper bounds
    string uppers;

    for(nodeIter = tempClasses.begin(); nodeIter != tempClasses.end(); nodeIter++)
    {
        QCTreeItem* node = *nodeIter;
        uppers += node->upperBound->toString();
    }


    cout<<"Actual Uppers  :"<<uppers<<endl;
    cout<<"Expected Uppers:"<<outputUppers<<endl;
    CPPUNIT_ASSERT(strcmp(uppers.c_str(),outputUppers.c_str())==0);
}

void Testcase1::testLowers(QCTree* tree,string outputLowers)
{

    vector<QCTreeItem*> tempClasses = tree->classes;

    vector <QCTreeItem*> :: const_iterator nodeIter;


    // Test the lower bounds

    string lowers;

    for(nodeIter = tempClasses.begin(); nodeIter != tempClasses.end(); nodeIter++)
    {
        QCTreeItem* node = *nodeIter;
        lowers += node->lowerBound->toString();
    }

    cout<<"Actual Lowers  :"<<lowers<<endl;
    cout<<"Expected Lowers:"<<outputLowers<<endl;
    CPPUNIT_ASSERT(strcmp(lowers.c_str(),outputLowers.c_str())==0);

}

void Testcase1::testAggregates(QCTree* tree,string aggregates)
{
    vector<QCTreeItem*> tempClasses = tree->classes;
    vector <QCTreeItem*> :: const_iterator nodeIter;

    // Test the upper bounds
    string agg;

    for(nodeIter = tempClasses.begin(); nodeIter != tempClasses.end(); nodeIter++)
    {
        QCTreeItem* node = *nodeIter;
        std::string s;
        std::stringstream out;
        out << node->upperBound->aggregate;
        s = out.str();
        agg +=  s+", ";
    }

    agg = agg.substr(0,agg.length()-2);

    cout<<"Actual Aggregates  :"<<agg<<endl;
    cout<<"Expected Aggregates:"<<aggregates<<endl;
    CPPUNIT_ASSERT(strcmp(agg.c_str(),aggregates.c_str())==0);
}

void Testcase1::testPointQuery(QCTree* tree,vector<string> v,double val)
{

    QCTreeQuery query;
    double agg = 0;
    long long unsigned int avg = 0;
    double min,max;
    Cell cell;
    bool testVal;

    cell.setDimensions(v);
    testVal = tree->query(v,&agg,&avg,&min,&max);

    if(val == -1)
    {
        CPPUNIT_ASSERT(testVal == false);
    }
    else
    {
        CPPUNIT_ASSERT(agg == val && testVal);
    }

}


void Testcase1::testRangeQuery(QCTree* tree,vector<string> v,double val)
{

    QCTreeQuery query;
    double agg = 0;
    long long unsigned int avg = 0;
    double min,max;
    Cell cell;
    bool testVal;

    cell.setDimensions(v);
    testVal = tree->query(v,&agg,&avg,&min,&max);

    if(val == -1)
    {
        CPPUNIT_ASSERT(testVal == false);
    }
    else
    {
        CPPUNIT_ASSERT(agg == val && testVal);
    }

}

void Testcase1::TestTemporaryClasses(void)
{

    QCTree *tree = new QCTree();

    tree->baseTable->addCol("a");
    tree->baseTable->addCol("b");
    tree->baseTable->addCol("c");
    tree->baseTable->addMeasure("sum");

    vector<string> v;

    tree->baseTable->addRow("Van b d1",9.0);
    tree->baseTable->addRow("Van f d2",3.0);
    tree->baseTable->addRow("Tor b d2",6.0);

    tree->constructTree();

    tree->printTempClasses();
    cout<<" ------------------------ TREE -------------------" <<endl;
    tree->printTree();

    vector<QCTreeItem*> tempClasses = tree->classes;
    vector <QCTreeItem*> :: const_iterator nodeIter;

    cout<<endl<<" ------------------------ Testcase -------------------" <<endl;

    // Test the upper bounds
    cout<<" ------------------------ Test Uppers -------------------" <<endl;

    string outputUppers = "<*,*,*><*,*,d2><*,b,*><Tor,b,d2><Tor,b,d2><Van,*,*><Van,b,d1><Van,b,d1><Van,b,d1><Van,b,d1><Van,f,d2><Van,f,d2><Van,f,d2>";
    testUppers(tree,outputUppers);

    // Test the lower bounds
    cout<<" ------------------------ Test Lowers -------------------" <<endl;

    string outputLowers ="<*,*,*><*,*,d2><*,b,*><Tor,*,*><*,b,d2><Van,*,*><Van,b,*><Van,*,d1><*,b,d1><*,*,d1><Van,f,*><Van,*,d2><*,f,*>";
    testLowers(tree,outputLowers);

    cout<<" ------------------------ Test Aggregates -------------------" <<endl;

    string aggreegates = "1 18 3 3 9 , 1 9 2 3 6 , 1 15 2 6 9 , 1 6 1 6 6 , 1 6 1 6 6 , 1 12 2 3 9 , 1 9 1 9 9 , 1 9 1 9 9 , 1 9 1 9 9 , 1 9 1 9 9 , 1 3 1 3 3 , 1 3 1 3 3 , 1 3 1 3 3 ";
    testAggregates(tree,aggreegates);

    cout<<" ------------------------ Testcase END-------------------" <<endl;

    TestQuery(tree);

    testSerialize(tree);

}

void Testcase1::TestQuery(QCTree* tree)
{

    cout<<"Test Point Queries:"<<endl;
    vector<string> v;
    v.clear();
    v.push_back("*");
    v.push_back("*");
    v.push_back("*");
    testPointQuery(tree,v,18);

    v.clear();
    v.push_back("Van");
    v.push_back("*");
    v.push_back("*");
    testPointQuery(tree,v,12);

    v.clear();
    v.push_back("Van");
    v.push_back("b");
    v.push_back("d1");
    testPointQuery(tree,v,9);

    v.clear();
    v.push_back("Van");
    v.push_back("f");
    v.push_back("d2");
    testPointQuery(tree,v,3);

    v.clear();
    v.push_back("Tor");
    v.push_back("*");
    v.push_back("*");
    testPointQuery(tree,v,6);

    v.clear();
    v.push_back("Tor");
    v.push_back("*");
    v.push_back("d2");
    testPointQuery(tree,v,6);

    v.clear();
    v.push_back("Tor");
    v.push_back("b");
    v.push_back("d2");
    testPointQuery(tree,v,6);

    v.clear();
    v.push_back("*");
    v.push_back("b");
    v.push_back("*");
    testPointQuery(tree,v,15);

    v.clear();
    v.push_back("*");
    v.push_back("*");
    v.push_back("d2");
    testPointQuery(tree,v,9);

    //Negative cases
    /*
    v.clear();
    v.push_back("Van");
    v.push_back("H");
    v.push_back("d2");
    testPointQuery(tree,v,-1);

    v.clear();
    v.push_back("Tor");
    v.push_back("f");
    v.push_back("d2");
    testPointQuery(tree,v,-1);

    v.clear();
    v.push_back("*");
    v.push_back("*");
    v.push_back("b");
    testPointQuery(tree,v,-1);
    */

    cout<<"Test Range Queries:"<<endl;

    v.clear();
    v.push_back("Tor Van");
    v.push_back("*");
    v.push_back("d2");
    testRangeQuery(tree,v,9);

    v.clear();
    v.push_back("Tor Van");
    v.push_back("b f");
    v.push_back("d1 d2");
    testRangeQuery(tree,v,18);

    v.clear();
    v.push_back("Van");
    v.push_back("b f");
    v.push_back("d1 d2");
    testRangeQuery(tree,v,12);

    v.clear();
    v.push_back("Van Tor");
    v.push_back("b f");
    v.push_back("d2");
    testRangeQuery(tree,v,9);

    v.clear();
    v.push_back("Van Tor");
    v.push_back("b");
    v.push_back("*");
    testRangeQuery(tree,v,15);
    /*
    v.clear();
    v.push_back("Tor");
    v.push_back("b");
    v.push_back("d1"); //Negative
    testRangeQuery(tree,v,-1);
    */


}


void Testcase1::TestTemporaryClasses1(void)
{

    QCTree *tree = new QCTree();

    tree->baseTable->addCol("a");
    tree->baseTable->addCol("b");
    tree->baseTable->addCol("c");
    tree->baseTable->addMeasure("sum");
    vector<string> v;
    tree->baseTable->addRow("S1 P1 s",6.0);
    tree->baseTable->addRow("S1 P2 s",12.0);
    tree->baseTable->addRow("S2 P1 f",9.0);

    tree->constructTree();


    tree->printTempClasses();
    cout<<" ------------------------ TREE -------------------" <<endl;
    tree->printTree();


    cout<<endl<<" ------------------------ Testcase -------------------" <<endl;

    cout<<" ------------------------ Test Uppers -------------------" <<endl;

    // Test the upper bounds
    string uppers = "<*,*,*><*,P1,*><S1,*,s><S1,*,s><S1,P1,s><S1,P1,s><S1,P2,s><S1,P2,s><S2,P1,f><S2,P1,f><S2,P1,f>";
    testUppers(tree,uppers);


    cout<<" ------------------------ Test Lowers -------------------" <<endl;
    // Test the lower bounds
    string lowers = "<*,*,*><*,P1,*><S1,*,*><*,*,s><S1,P1,s><*,P1,s><S1,P2,s><*,P2,*><S2,*,*><*,P1,f><*,*,f>";
    testLowers(tree,lowers);

    cout<<" ------------------------ Test Aggregates -------------------" <<endl;

    string aggreegates = "1 27 3 6 12 , 1 15 2 6 9 , 1 18 2 6 12 , 1 18 2 6 12 , 1 6 1 6 6 , 1 6 1 6 6 , 1 12 1 12 12 , 1 12 1 12 12 , 1 9 1 9 9 , 1 9 1 9 9 , 1 9 1 9 9 ";
    testAggregates(tree,aggreegates);

    //testSerialize(tree);
}

void Testcase1::testSerialize(QCTree* tree)
{

    tree->serialize();

    QCTree* newTree = new QCTree();

    newTree->deserialize();

    TestQuery(newTree);

    delete newTree;
}
