#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class VariantTree : public QObject
{
    Q_OBJECT

public:
    VariantTree();
    ~VariantTree();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

VariantTree::VariantTree()
{

}

VariantTree::~VariantTree()
{

}

void VariantTree::initTestCase()
{

}

void VariantTree::cleanupTestCase()
{

}

void VariantTree::test_case1()
{

}

QTEST_MAIN(VariantTree)

#include "tst_varianttree.moc"
