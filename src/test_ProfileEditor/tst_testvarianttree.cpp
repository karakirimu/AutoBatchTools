#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include <../ProfileEditor/profileeditor.h>

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

private:
    ProfileEditor *profile;

};

testVariantTree::testVariantTree()
{

}

testVariantTree::~testVariantTree()
{

}

void testVariantTree::initTestCase()
{
    profile = new ProfileEditor();
    profile->show();
    QTest::qWait(1000);
}

void testVariantTree::cleanupTestCase()
{

}

void testVariantTree::test_case1()
{

}

QTEST_MAIN(testVariantTree)

#include "tst_testvarianttree.moc"
