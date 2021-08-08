/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef TST_UNITTEST_H
#define TST_UNITTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <xmlbuilder.h>
#include <searchxmlbuilder.h>
#include <profilexmlbuilder.h>
#include <settingcache/startupxmlbuilder.h>
#include <pluginsxmlbuilder.h>
#include <stringxmlbuilder.h>
#include <processxmlbuilder.h>

using namespace testing;

//#include <filesearchloader.h>
//class FileSearchLoaderTest : public ::testing::Test {

//protected:
//    virtual void SetUp() {
////        setInitialList();
//    };

//    virtual void TearDown() {
//    };

//    QList<QStringList> itemlist;

//private:
//    void setInitialList(){
//        QStringList tmp;

//        //add title
//        itemlist.append(QStringList() << SEARCH_NAME << "");

//        //add keyword
//        itemlist.append(QStringList() << SEARCH_KEYWORD << "");

//        //add regex
//        tmp << SEARCH_REGEX << ""/*string*/
//            << ENABLED << ""/*bool*/;
//        itemlist.append(tmp);
//        tmp.clear();

//        //add dir
//        itemlist.append(QStringList() << SEARCH_DIR << "");

//        //add recursive
//        itemlist.append(QStringList() << SEARCH_RECURSIVE \
//                    << ""/*bool*/);

//        //add seconds
//        tmp << SEARCH_SECONDS << ""/*int*/
//            << ENABLED << ""/*bool*/;
//        itemlist.append(tmp);
//        tmp.clear();

//        //add creation
//        tmp << SEARCH_CREATION << ""/*datetime*/
//            << ENABLED << ""/*bool*/
//            << COMBO << "" /*index of combobox*/;
//        itemlist.append(tmp);
//        tmp.clear();

//        //add modified
//        tmp << SEARCH_MODIFIED << ""/*datetime*/
//            << ENABLED << ""/*bool*/
//            << COMBO << ""/*index of combobox*/;
//        itemlist.append(tmp);
//        tmp.clear();

//        //add filesize_1(fsize_1)
//        tmp << SEARCH_FSIZE_1 << ""/*bytes*/
//            << ENABLED << ""/*bool*/
//            << COMBO << ""/*index of combobox*/;
//        itemlist.append(tmp);
//        tmp.clear();

//        //add filesize_2(fsize_2)
//        tmp << SEARCH_FSIZE_2 << ""/*bytes*/
//            << ENABLED << ""/*bool*/
//            << COMBO << ""/*index of combobox*/;
//        itemlist.append(tmp);
//        tmp.clear();
//    }

////    void createfile(QString filename){
////        QFile tmpfilemake(filename);
////        if(tmpfilemake.open(QIODevice::WriteOnly)){
////            tmpfilemake.close();
////        }
////    }

////    // make test files and folder
////    void createTestCandidate(){
////        QString temp = "TestFolder";
////        _mkdir(temp.toStdString().c_str());
////        createfile(temp + "/test.txt");
////    }
//};

//TEST_F(FileSearchLoaderTest, unittest)
//{


//    EXPECT_EQ(1, 1);
//    ASSERT_THAT(0, Eq(0));

////    FileSearchLoader loader;
////    loader.searchFromStrList()
//}


//#include <basefilesearch.h>
//class BaseFileSearchTest : public ::testing::Test {

//protected:
//    virtual void SetUp() {
//    };

//    virtual void TearDown() {
//    };

//private:
//};

class ProcessXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new ProcessXmlBuilder();
        QDir dir;
        dir.mkdir("./test");
        builder->setLoadPath(QDir::currentPath() + "/test/proc.apro");
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir(QDir::currentPath() + "/test");
        dir.removeRecursively();
    }

    const QList<QStringList> GetInfoElements(){
        QList<QStringList> res;

        //info element
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::INFORMATION)));
        res.append((QStringList() << pxc.TAG_I_NAME << ""));
        res.append((QStringList() << pxc.TAG_I_VERSION << ""));
        res.append((QStringList() << pxc.TAG_I_AUTHOR << ""));
        res.append((QStringList() << pxc.TAG_I_DESCRIPTION << ""));
        res.append((QStringList() << pxc.TAG_I_FILEINPUT_BOOL << "yes"));
        res.append((QStringList() << pxc.TAG_I_FILEINPUT_SEARCH_BOOL << "no"));
        res.append((QStringList() << pxc.TAG_I_FILESEARCH_NAME << ""
                                      << pxc.ATTR_COMMAND_ID_INT << ""));
        res.append((QStringList() << pxc.TAG_I_PROCESS_BOOL_HA1 << "yes"
                                      << pxc.ATTR_I_PROCESSMAX_INT << "1"));
        res.append((QStringList() << pxc.TAG_I_ARG_IN_ONELOOP_INT << "1"));
        res.append((QStringList() << pxc.TAG_I_RECURSIVE_LOOPMAX_INT << "0"));
        res.append((QStringList() << pxc.TAG_I_PROFILE_BASEPATH << "./"));

        return res;
    }

    const QList<QStringList> GetLocalElements(){
        QList<QStringList> res;

        //local element
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::LOCAL)));
        res.append((QStringList() << pxc.TAG_L_VARIANTCOUNT_INT << "1"));
        res.append(QStringList() << pxc.TAG_L_VARIANT_HA1 << "v.variant"
                                 << pxc.ATTR_L_VALUE << "v.value");

        return res;
    }

    const QList<QStringList> GetExecutionElements(){
        QList<QStringList> res;

        // execution element
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::EXECUTE)
                                  << pxc.ATTR_ONLY_SCHEDULER_BOOL
                                  << "true"));
        res.append((QStringList() << pxc.TAG_E_TIMEOUT_BOOL_HA1
                                  << "true"
                                  << pxc.ATTR_TIMEOUT_INT
                                  << QString::number(23504)));
        res.append((QStringList() << pxc.TAG_E_DETACH_BOOL
                                  << "false"));

        qsizetype commandCount = 3;
        res.append((QStringList() << pxc.TAG_E_COMMANDCOUNT_INT
                                  << QString::number(commandCount)));

        for(int i = 0; i < commandCount; i++){
            res.append(QStringList() << pxc.TAG_E_CMD_HA1
                                     << QString("dummy${%1}").arg(i)
                                     << pxc.ATTR_COMMAND_ID_INT
                                     << QString::number(i));
        }

        return res;
    }

    const QList<QStringList> GetFileSearchElements(){
        QList<QStringList> res;

        // filesearch elements
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::FILESEARCH)
                                  << pxc.ATTR_ONLY_SCHEDULER_BOOL
                                  << "true"));
        res.append((QStringList() << pxc.TAG_FS_NAME_HA1
                                  << "simple search"
                                  << pxc.ATTR_COMMAND_ID_INT
                                  << QString::number(2)));
        res.append((QStringList() << pxc.TAG_FS_SEPARATOR << ","));
        res.append((QStringList() << pxc.TAG_FS_VARIANT << "store"));
        res.append((QStringList() << pxc.TAG_FS_FILEPATH_HA1
                                  << "../"
                                  << pxc.ATTR_FS_OUTPUTOPTION_INT
                                  << QString::number(0)));
        res.append((QStringList() << pxc.TAG_FS_WRITEOPTION_INT
                                  << QString::number(1)));

        return res;
    }

    const QList<QStringList> GetPluginElements() {
        QList<QStringList> res;

        // plugin elements
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::PLUGIN)
                                  << pxc.ATTR_ONLY_SCHEDULER_BOOL
                                  << "true"));
        res.append((QStringList() << pxc.TAG_P_NAME << "plugin@name"));
        res.append((QStringList() << pxc.TAG_P_FILEPATH
                                  << "./fff/ggg/vvvv.dll"));

        int commandCount = 5;
        res.append((QStringList() << pxc.TAG_P_COMMANDCOUNT_INT
                                  << QString::number(commandCount)));

        for(int i = 0; i < commandCount; i++){
            res.append(QStringList() << pxc.TAG_P_CMD_HA1
                                     << QString("plugindummy${%1}").arg(i)
                                     << pxc.ATTR_COMMAND_ID_INT
                                     << QString::number(i));
        }

        return res;
    }

    const QList<QStringList> GetProfileLoadElements() {

        QList<QStringList> res;
        // profileload elements
        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::PROFILELOAD)
                                  << pxc.ATTR_ONLY_SCHEDULER_BOOL
                                  << "true"));
        res.append((QStringList() << pxc.TAG_PLOAD_FILEPATH
                                  << "./plugins/test.dll"));

        return res;
    }

    const QList<QStringList> GetTempElements() {
        QList<QStringList> res;

        res.append((QStringList() << pxc.TAG_TYPE
                                  << fs.getString(fs.TYPE::ALLINCLUDE)));
        res.append((QStringList() << pxc.TAG_FUNCTIONSELECT
                                  << QString::number(2)));


        res.append(GetExecutionElements());
        res.append(GetFileSearchElements());
        res.append(GetPluginElements());
        res.append(GetProfileLoadElements());

        return res;
    }

    ProcessXmlBuilder *builder;
private:
    ProcessXmlConstant pxc;
    FunctionType fs;
};

TEST_F(ProcessXmlBuilderTest, ReadItem){
    constexpr int count = 10;
    QList<QList<QStringList> *> lists;

    lists.append(new QList<QStringList>(GetInfoElements()));
    lists.append(new QList<QStringList>(GetLocalElements()));
    lists.append(new QList<QStringList>(GetExecutionElements()));
    lists.append(new QList<QStringList>(GetFileSearchElements()));
    lists.append(new QList<QStringList>(GetPluginElements()));

    lists.append(new QList<QStringList>(GetProfileLoadElements()));
    lists.append(new QList<QStringList>(GetTempElements()));
    lists.append(new QList<QStringList>(GetTempElements()));
    lists.append(new QList<QStringList>(GetTempElements()));
    lists.append(new QList<QStringList>(GetTempElements()));

    builder->writeAllItem(&lists);

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, GetInfoElements());

    readlist.clear();
    builder->readItem(1, &readlist);
    EXPECT_EQ(readlist, GetLocalElements());

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, GetTempElements());

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, GetProfileLoadElements());

    QList<QList<QStringList> *> readlists;
    builder->readAllItem(&readlists);

    EXPECT_EQ(*readlists.at(0), GetInfoElements());
    EXPECT_EQ(*readlists.at(1), GetLocalElements());
    EXPECT_EQ(*readlists.at(2), GetExecutionElements());
    EXPECT_EQ(*readlists.at(3), GetFileSearchElements());
    EXPECT_EQ(*readlists.at(4), GetPluginElements());
    EXPECT_EQ(*readlists.at(5), GetProfileLoadElements());
    EXPECT_EQ(*readlists.at(6), GetTempElements());
    EXPECT_EQ(*readlists.at(7), GetTempElements());
    EXPECT_EQ(*readlists.at(8), GetTempElements());
    EXPECT_EQ(*readlists.at(9), GetTempElements());
}

class PluginsXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new PluginsXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir("./plugins");
        dir.removeRecursively();
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;

        res.append(QStringList() << builder->PL_XML_NAME
                                 << "from->settingName"
                                 << builder->PL_XML_ATTR_FILE
                                 << "./filepath/");

        return res;
    }

    PluginsXmlBuilder *builder;
private:
};

TEST_F(PluginsXmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

class StringXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new StringXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir("./settings");
        dir.removeRecursively();
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;
        QStringList list = QStringList() << "test" << "#sss";
        builder->createVarElement(&res, &list);
        return res;
    }

    StringXmlBuilder *builder;
private:
    SchedulerXmlConstant sxc;
};

TEST_F(StringXmlBuilderTest, CreateLists){

    QList<QStringList> res;
    QStringList list = QStringList() << "test" << "#sss";
    builder->createVarElement(&res, &list);

    QList<QStringList> expected;
    expected.append(QStringList() << "variant" << list.at(0));
    expected.append(QStringList() << "value" << list.at(1));
    EXPECT_EQ(res, expected);

    // value is not set
    res.clear();
    QStringList empty;
    builder->createVarElement(&res, &empty);
    QList<QStringList> expected2;
    EXPECT_EQ(res, expected2);

    // It has already added values.
    QStringList list2 = QStringList() << "sfv" << "#fff";
    builder->createVarElement(&res, &list);
    builder->createVarElement(&res, &list2);
    EXPECT_EQ(res, expected);
}

TEST_F(StringXmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

class StartupXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new StartupXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir("./settings");
        dir.removeRecursively();
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;

        res.append(QStringList() << sxc.TAG_NAME << "from->settingName");
        res.append(QStringList() << sxc.TAG_PROFILE << "./setting\\/");
        res.append(QStringList() << sxc.TAG_SCHEDULED \
                                << "true");
        res.append(QStringList() << sxc.TAG_SCHEDULETYPE \
                                << QString::number(1));
        res.append(QStringList() << sxc.TAG_DATETIME \
                                << "2025/7/2 12:51:24");
        res.append(QStringList() << sxc.TAG_SECOND \
                                << QString::number(45000));
        res.append(QStringList() << sxc.TAG_TIME << "17:23:05");
        res.append(QStringList() << sxc.TAG_DATE << "20010100");

        return res;
    }

    StartupXmlBuilder *builder;
private:
    SchedulerXmlConstant sxc;
};

TEST_F(StartupXmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

class ProfileXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new ProfileXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir("./settings");
        dir.removeRecursively();
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;
        res.append(QStringList() << PROFILE_NAME << "profiletest");
        res.append(QStringList() << PROFILE_DESCRIPTION << "profiledesc");
        res.append(QStringList() << PROFILE_FILE << "./filetest");

        return res;
    }

    ProfileXmlBuilder *builder;
};

TEST_F(ProfileXmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

TEST_F(ProfileXmlBuilderTest, CreateLists){

    QList<QStringList> res;
    QStringList list = QStringList() << "profiletest"
                                     << "profiledesc"
                                     << "./filetest";
    builder->createVarElement(&res, &list);

    QList<QStringList> expected;
    expected.append(QStringList() << PROFILE_NAME << list.at(0));
    expected.append(QStringList() << PROFILE_DESCRIPTION << list.at(1));
    expected.append(QStringList() << PROFILE_FILE << list.at(2));
    EXPECT_EQ(res, expected);

    // Value is not set, this returns empty string.
    res.clear();
    QStringList empty;
    builder->createVarElement(&res, &empty);
    QList<QStringList> expected2;
    EXPECT_EQ(res, expected2);

    // This function will not append a value more than once.
    QStringList list2 = QStringList() << "sfv" << "#fff";
    builder->createVarElement(&res, &list);
    builder->createVarElement(&res, &list2);
    EXPECT_EQ(res, expected);
}

class SearchXmlBuilderTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        builder = new SearchXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        QDir dir("./settings");
        dir.removeRecursively();
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;
        QStringList tmp;
        //add title
        res.append(QStringList() << SEARCH_NAME << "unittest");

        //add keyword
        res.append(QStringList() << SEARCH_KEYWORD << "*.*");

        //add regex
        tmp << SEARCH_REGEX << "([A-Z])\\w+"/*string*/
            << ENABLED << "true"/*bool*/;
        res.append(tmp);
        tmp.clear();

        //add dir
        res.append(QStringList() << SEARCH_DIR << "./test");

        //add recursive
        res.append(QStringList() << SEARCH_RECURSIVE << "true"/*bool*/);

        //add seconds
        tmp << SEARCH_SECONDS << "23456"/*int*/
            << ENABLED << "true"/*bool*/;
        res.append(tmp);
        tmp.clear();

        //add creation
        tmp << SEARCH_CREATION << "2023/2/8 15:22:30"/*datetime*/
            << ENABLED << "false"/*bool*/
            << COMBO << "1" /*index of combobox*/;
        res.append(tmp);
        tmp.clear();

        //add modified
        tmp << SEARCH_MODIFIED << "2025/7/2 12:51:24"/*datetime*/
            << ENABLED << "true"/*bool*/
            << COMBO << "4"/*index of combobox*/;
        res.append(tmp);
        tmp.clear();

        //add filesize_1(fsize_1)
        tmp << SEARCH_FSIZE_1 << "50"/*bytes*/
            << ENABLED << "false"/*bool*/
            << COMBO << "0"/*index of combobox*/;
        res.append(tmp);
        tmp.clear();

        //add filesize_2(fsize_2)
        tmp << SEARCH_FSIZE_2 << "30000000"/*bytes*/
            << ENABLED << "false"/*bool*/
            << COMBO << "2"/*index of combobox*/;
        res.append(tmp);
        tmp.clear();

        return res;
    }

    SearchXmlBuilder *builder;
};

TEST_F(SearchXmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

// Dummy class for xmlbuilder
class MockXmlBuilder : public Xmlbuilder {
//    Q_OBJECT
public:
    explicit MockXmlBuilder(QObject *parent = nullptr) {
        //set new folder
        QDir dir;
        dir.mkdir("./temp");

        //set new file
        setFileName("./temp/xmltest");

        if(count() == 0){
            createDocument();
        }
    }
    virtual ~MockXmlBuilder() override {}

    bool readAll(QList<QList<QStringList> *> *itemlist){
        return Xmlbuilder::readAllItem(FIRSTLAYER
                                       , ATTR
                                       , itemlist);
    }

    bool writeAll(const QList<QList<QStringList> *> *itemlist){
        return Xmlbuilder::writeAllItem(ROOTELEMENT
                                        , ROOTATTR
                                        , ROOTATTRVALUE
                                        , FIRSTLAYER
                                        , ATTR
                                        , itemlist);
    }

    bool readItem(int itemid, QList<QStringList> *itemlist)
    {
        return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
    }

    // overwriteitem in BaseXmlBuilder
    bool addItem(const QList<QStringList> *itemlist)
    {
        return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
    }

    bool deleteItem(int itemid)
    {
        return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
    }

    bool editItem(int itemid, const QList<QStringList> *itemlist)
    {
        return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
    }

    void swapItem(int beforeitemid, int afteritemid)
    {
        Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
    }

    // this function needs to call setrowcount
    void copyItem(int itemid)
    {
        Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, COPY_NAME);
    }

    void createDocument()
    {
        createXmlBaseDocument(ROOTELEMENT);
    }

    int count()
    {
        return getElementItemsCount(FIRSTLAYER);
    }

    void setSearchItemData(QString element
                           , QList<QStringList> *list) override {

#ifdef QT_DEBUG
//        if(element == (TEST_TAG + "0")
//            || element == (TEST_TAG + "1")
//            || element == (TEST_TAG + "2")
//            || element == (TEST_TAG + "3")
//            || element == (TEST_TAG + "4")) {

//            const QString classname = "[DummyXmlBuilder::setSearchItemData] ";
//            qDebug() << classname << "Element : "
//                     << element << rxml->readElementText();
//            qDebug() << classname << "Name : " << rxml->name();
//            qDebug() << classname << "Attributes count : "
//                     << rxml->attributes().size();
//            for(auto attr : rxml->attributes()){
//                qDebug() << classname << "Attribute name : "
//                         << attr.name();
//                qDebug() << classname << "Attribute value : "
//                         << attr.value();
//            }
//        }
#endif

        if(element == (TEST_TAG + "0")
            || element == (TEST_TAG + "1")
            || element == (TEST_TAG + "2")
            || element == (TEST_TAG + "3")
            || element == (TEST_TAG + "4")) {

            // Recently, QXmlStreamReader changed.
            // It has to read left to right, therefore read attribute first.
            QStringList read;

            for(auto &attr : rxml->attributes()){
                read.append(attr.name().toString());
                read.append(attr.value().toString());
            }

            // Using readElementText will make the corresponding tag unreadable.
            read.prepend(rxml->readElementText());
            read.prepend(element);

            list->append(read);
        }

    }
private:
    const QString ROOTELEMENT   = "test";
    const QString ROOTATTR      = "version";
    const QString ROOTATTRVALUE = "1.0";
    const QString FIRSTLAYER  = "item";
    const QString ATTR = "id";

    const QString COPY_NAME = "name";

    const QString TEST_TAG = "tag";
    const QString TEST_ATTR = "attr";
};

class XmlBuilderTest : public ::testing::Test {

protected:
    virtual void SetUp() override {
        builder = new MockXmlBuilder();
    }

    virtual void TearDown() override {
        delete builder;

        if(testfile.isOpen()){
            testfile.close();
        }

        QDir dir("./temp");
        dir.removeRecursively();
    }

    void OpenTestFile(){
        // The testfile is automatically delete in TearDown.
        testfile.setFileName("./temp/xmltest");
        if(testfile.exists()){
            bool opened = testfile.open(QFile::ReadOnly);
            ASSERT_TRUE(opened);
        }
    }

//    void CloseTestFile(){
//        if(testfile->isOpen()){
//            testfile->close();
//        }
//    }

    QString ReadAllTestFile(){
        try{
            return QString::fromUtf8(testfile.readAll());
        }catch(int *){
            throw "testfile is nullptr\n";
        }
    }

    const QList<QStringList> GetTestElements(){
        QList<QStringList> res;

        res.append(QStringList() << "tag0" << "content0");

        res.append(QStringList() << "tag1" << "content1"
                                 << "attr0" << "attrcontent0");

        res.append(QStringList() << "tag2" << "content2"
                                 << "attr0" << "attrcontent0"
                                 << "attr1" << "attrcontent1");

        res.append(QStringList() << "tag3" << "content3"
                                 << "attr0" << "attrcontent0"
                                 << "attr1" << "attrcontent1"
                                 << "attr2" << "attrcontent2");

        res.append(QStringList() << "tag4" << "content4"
                                 << "attr0" << "attrcontent0"
                                 << "attr1" << "attrcontent1"
                                 << "attr2" << "attrcontent2"
                                 << "attr3" << "attrcontent3");

        return res;
    }

    const QList<QStringList> GetEditTestElements(){
        QList<QStringList> res;

        res.append(QStringList() << "tag0" << "edit0");

        res.append(QStringList() << "tag1" << "edit1"
                                 << "attr0" << "attredit0-0");

        res.append(QStringList() << "tag2" << "edit2"
                                 << "attr0" << "attredit1-0"
                                 << "attr1" << "attredit1-1");

        res.append(QStringList() << "tag3" << "edit3"
                                 << "attr0" << "attredit2-0"
                                 << "attr1" << "attredit2-1"
                                 << "attr2" << "attredit2-2");

        res.append(QStringList() << "tag4" << "edit4"
                                 << "attr0" << "attredit3-0"
                                 << "attr1" << "attredit3-1"
                                 << "attr2" << "attredit3-2"
                                 << "attr3" << "attredit3-3");
        return res;
    }

    const QString GetEvaluateString(int count){
        QString correct = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                                  "<test>\r\n");

        for(int i = 0; i < count; i++){
            QString content = GetItemEvaluate(i);
            correct.append(content);
        }

        correct.append("</test>\r\n");

        return correct;
    }

    const QString GetEvaluateStringWithFirstLayerAttribute(int count){
        QString correct = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                                  "<test version=\"1.0\">\r\n");

        for(int i = 0; i < count; i++){
            QString content = GetItemEvaluate(i);
            correct.append(content);
        }

        correct.append("</test>\r\n");

        return correct;
    }

    const QString GetItemEvaluate(int index){
        return QString("\t<item id=\"%1\">\r\n"
                       "\t\t<tag0>content0</tag0>\r\n"
                       "\t\t<tag1 attr0=\"attrcontent0\">content1</tag1>\r\n"
                       "\t\t<tag2 attr0=\"attrcontent0\" attr1=\"attrcontent1\">content2</tag2>\r\n"
                       "\t\t<tag3 attr0=\"attrcontent0\" attr1=\"attrcontent1\" attr2=\"attrcontent2\">content3</tag3>\r\n"
                       "\t\t<tag4 attr0=\"attrcontent0\" attr1=\"attrcontent1\" attr2=\"attrcontent2\" attr3=\"attrcontent3\">content4</tag4>\r\n"
                       "\t</item>\r\n").arg(index);
    }

    const QString GetItemEditEvaluate(int index){
        return QString("\t<item id=\"%1\">\r\n"
                       "\t\t<tag0>edit0</tag0>\r\n"
                       "\t\t<tag1 attr0=\"attredit0-0\">edit1</tag1>\r\n"
                       "\t\t<tag2 attr0=\"attredit1-0\" attr1=\"attredit1-1\">edit2</tag2>\r\n"
                       "\t\t<tag3 attr0=\"attredit2-0\" attr1=\"attredit2-1\" attr2=\"attredit2-2\">edit3</tag3>\r\n"
                       "\t\t<tag4 attr0=\"attredit3-0\" attr1=\"attredit3-1\" attr2=\"attredit3-2\" attr3=\"attredit3-3\">edit4</tag4>\r\n"
                       "\t</item>\r\n").arg(index);
    }

    void EvaluateNestedStringList(QList<QStringList> *a
                                  , QList<QStringList> *b){

        // QList count check
        ASSERT_EQ(a->count(), b->count());

        int size = a->count();

        // QStringList count check
        for(int i = 0; i < size; i++){
            ASSERT_EQ(a->at(i).count(), b->at(i).count());

            int stringlistsize = a->at(i).count();
            for(int j = 0; j < stringlistsize; j++){
                EXPECT_EQ(a->at(i).at(j).toStdString()
                        , b->at(i).at(j).toStdString());
            }
        }
    }

    MockXmlBuilder *builder;
private:
    QFile testfile;
};

TEST_F(XmlBuilderTest, CreateInitialDocument)
{
    OpenTestFile();

    QString all = ReadAllTestFile();
#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
#endif

    QString correct = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                                  "<test>\r\n"
                                  "</test>\r\n");
#if WIN32
        /* CRLF */
        EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
        /* LF */
        QString correctu = correct.replace("\r\n","\n");
        EXPECT_EQ(all.toStdString(), correctu.toStdString());
#endif
}

TEST_F(XmlBuilderTest, WriteItem){
    QList<QStringList> list = GetTestElements();
    builder->addItem(&list);
    OpenTestFile();

    QString all = ReadAllTestFile();
#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
#endif
    QString correct = GetEvaluateString(1);

#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all.toStdString(), correctu.toStdString());
#endif
}

TEST_F(XmlBuilderTest, WriteItemMultiTimes){
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    OpenTestFile();

    QString all = ReadAllTestFile();
#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
#endif
    QString correct = GetEvaluateString(count);

#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all, correctu);
#endif
}

TEST_F(XmlBuilderTest, EditItem){
    constexpr int count = 1;
    // write item
    QList<QStringList> list = GetTestElements();
    builder->addItem(&list);

    // edititem (all contents)
    QList<QStringList> res = GetEditTestElements();
    builder->editItem(0, &res);

    OpenTestFile();

    QString all = ReadAllTestFile();
#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
#endif
    QString correct = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                              "<test>\r\n");

    for(int i = 0; i < count; i++){
        QString content = GetItemEditEvaluate(i);
        correct.append(content);
    }

    correct.append("</test>\r\n");

#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all, correctu);
#endif
}

TEST_F(XmlBuilderTest, ReadItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // read first, last and others
    QList<QStringList> readlist;
    builder->readItem(0, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count - 1, &readlist);
    EXPECT_EQ(readlist, list);

    readlist.clear();
    builder->readItem(count/2, &readlist);
    EXPECT_EQ(readlist, list);
}

TEST_F(XmlBuilderTest, DeleteItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // Edit some elements which before delete.
    QList<QStringList> res = GetEditTestElements();
    builder->editItem(0, &res);
    builder->editItem(count / 2, &res);
    builder->editItem(count - 1, &res);

    builder->deleteItem(count - 1);
    builder->deleteItem(count / 2);
    builder->deleteItem(0);

    OpenTestFile();
    QString all = ReadAllTestFile();
    QString correct = GetEvaluateString(count - 3);
#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all, correctu);
#endif
}

TEST_F(XmlBuilderTest, SwapItem){
    // write item
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    // Edit element.
    QList<QStringList> res = GetEditTestElements();
    builder->editItem(0, &res);

    // moved check
    for(int i = 1; i < count; i++){
        builder->swapItem(i - 1, i);

        QList<QStringList> original;
        builder->readItem(i - 1, &original);
        EvaluateNestedStringList(&list, &original);

        QList<QStringList> edited;
        builder->readItem(i, &edited);
        EvaluateNestedStringList(&res, &edited);
    }

    // Check last
    OpenTestFile();
    QString all = ReadAllTestFile();
    QString correct = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                              "<test>\r\n");

    for(int i = 0; i < count; i++){
        QString content = "";

        if(i == (count - 1)){
            content = GetItemEditEvaluate(i);
        }else{
            content = GetItemEvaluate(i);
        }
        correct.append(content);
    }

    correct.append("</test>\r\n");

#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
    std::cout << correct.toStdString() << std::endl;
#endif

#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all, correctu);
#endif
}

TEST_F(XmlBuilderTest, Count){
    constexpr int count = 10;

    QList<QStringList> list = GetTestElements();
    for(int i = 0; i < count; i++){
        builder->addItem(&list);
    }

    EXPECT_EQ(count, builder->count());
}

TEST_F(XmlBuilderTest, WriteAll){
    constexpr int count = 10;
    QList<QList<QStringList> *> lists;
    QList<QStringList> list = GetTestElements();

    for(int i = 0; i < count; i++){
        lists.append(new QList<QStringList>(list));
    }

    builder->writeAll(&lists);

    OpenTestFile();

    QString all = ReadAllTestFile();
#ifdef QT_DEBUG
    std::cout << all.toStdString() << std::endl;
#endif
    QString correct = GetEvaluateStringWithFirstLayerAttribute(count);

#if WIN32
    /* CRLF */
    EXPECT_EQ(all.toStdString(), correct.toStdString());
#else
    /* LF */
    QString correctu = correct.replace("\r\n","\n");
    EXPECT_EQ(all.toStdString(), correctu.toStdString());
#endif
}

TEST_F(XmlBuilderTest, ReadAll){
    constexpr int count = 10;
    QList<QList<QStringList> *> lists;
    QList<QStringList> list = GetTestElements();

    for(int i = 0; i < count; i++){
        lists.append(new QList<QStringList>(list));
    }

    builder->writeAll(&lists);

    QList<QList<QStringList> *> readlists;
    builder->readAll(&readlists);

    for(int i = 0; i < count; i++){
        QList<QStringList> *read = readlists.at(i);
        EXPECT_EQ(list, *read);
    }
}

#endif // TST_UNITTEST_H
