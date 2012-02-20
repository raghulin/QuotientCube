#ifndef TESTCASE1_H
#define TESTCASE1_H

#include <QCTree.h>

#include <cppunit/extensions/HelperMacros.h>

class Testcase1 :public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Testcase1);
    CPPUNIT_TEST(TestTemporaryClasses);
    CPPUNIT_TEST(TestTemporaryClasses1);
    CPPUNIT_TEST_SUITE_END();

public:

    Testcase1()
    {

    }

    void setUp(void) {}    // I don't use setUp or tearDown yet, but let's
    void tearDown(void) {} // leave them in so I remember their names

    void testLowers(QCTree* tree,string outputLowers);
    void testUppers(QCTree* tree,string outputUppers);
    void testAggregates(QCTree* tree,string aggregates);

    void testPointQuery(QCTree* tree,vector<string> v,double val);
    void testRangeQuery(QCTree* tree,vector<string> v,double val);
    void testSerialize(QCTree* tree);

    void TestTemporaryClasses(void);
    void TestTemporaryClasses1(void);

    void TestQuery(QCTree* tree);

};

#endif // TESTCASE1_H
