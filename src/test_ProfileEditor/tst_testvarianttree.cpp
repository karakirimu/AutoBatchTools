#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class testVariantTree : public QObject
{
    Q_OBJECT

public:
    testVariantTree();
    ~testVariantTree();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

testVariantTree::testVariantTree()
{

}

testVariantTree::~testVariantTree()
{

}

void testVariantTree::initTestCase()
{

}

void testVariantTree::cleanupTestCase()
{

}

void testVariantTree::test_case1()
{

}

QTEST_MAIN(testVariantTree)

#include "tst_testvarianttree.moc"
