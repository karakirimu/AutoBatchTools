/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef XMLLISTGENERATOR_TEST_H
#define XMLLISTGENERATOR_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../share/xmlresolver.h"
#include "../share/process/processxmlconstant.h"

using namespace testing;

class XmlResolverTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        resetTestItems();
    }

    virtual void TearDown() override {
        test.clear();
    }

    void resetTestItems(){
        test.clear();

        //set item 1 to 100
        //  1  2  3  4  5  6  7  8  9  10
        // 11 12 13 14 ..
        for(int i = 0; i < rows; i++){
            QStringList temp;
            for(int j = 0; j < cols; j++){
                temp.append(QString::number((i * cols) + (j + 1)));
            }
            test.append(temp);
        }
    }

    void createSameTestItems(){
        test.clear();

        //set item 1 to 100
        //  1  2  3  4  5  6  7  8  9  10
        //  1 12  3 14  5 16 ..
        //  1 22  3 24  5 26 ..
        for(int i = 0; i < rows; i++){
            QStringList temp;
            for(int j = 0; j < cols; j++){
                if(j % 2 == 0){
                    temp.append(QString::number(j + 1));
                }else{
                    temp.append(QString::number((i * cols) + (j + 1)));
                }
            }
            test.append(temp);
        }
    }

    XmlResolver pxc;
    QList<QStringList> test;
    static constexpr int rows = 10;
    static constexpr int cols = 10;
};

TEST_F(XmlResolverTest, fetchTag){
    for(const QStringList &ql : test){
        QString val = pxc.fetch(&test, ql.first());
        EXPECT_EQ(ql.at(1), val);
    }
}

TEST_F(XmlResolverTest, fetchAttribute){
    for(const QStringList &ql : test){
        for(int i = 2; i < cols; i += 2){
            QString val = pxc.fetch(&test, ql.first(), ql.at(i));
            EXPECT_EQ(ql.at(i+1), val);
        }
    }
}

TEST_F(XmlResolverTest, fetchTagValue){
    for(const QStringList &ql : test){
        QString val = pxc.fetch(&test, ql.first(), "", ql.at(1));
        EXPECT_EQ(ql.at(1).toStdString(), val.toStdString());
    }
}

TEST_F(XmlResolverTest, fetchTagValueAttribute){
    // same candidate with different value.
    createSameTestItems();
    for(const QStringList &ql : test){
        for(int i = 2; i < cols; i += 2){
            QString val = pxc.fetch(&test, ql.first(), ql.at(i), ql.at(1));
            EXPECT_EQ(ql.at(i+1), val);
        }
    }
}

TEST_F(XmlResolverTest, fetchTagFirstPosition){
    int half = static_cast<int>(test.count()) / 2;

    for(int i = 0; i < rows; i++){
        QString val = pxc.fetch(&test, test.at(half).first(), "", "", i);

        if(i <= half){
            EXPECT_EQ(test.at(half).at(1).toStdString(), val.toStdString());
        }else{
            EXPECT_EQ("", val.toStdString());
        }
    }
}

TEST_F(XmlResolverTest, fetchCommandFirstPos){
    int res = pxc.fetchCommandFirstPos("invalid", &test);
    EXPECT_EQ(-1, res);

    ProcessXmlConstant c;
    test.insert(1, QStringList() << c.TAG_E_COMMANDCOUNT_INT << "1");

    test.insert(5, QStringList() << c.TAG_P_COMMANDCOUNT_INT << "0");

    res = pxc.fetchCommandFirstPos(c.TAG_E_CMD_HA1, &test);
    EXPECT_EQ(2, res);

    res = pxc.fetchCommandFirstPos(c.TAG_E_COMMANDCOUNT_INT, &test);
    EXPECT_EQ(2, res);

    res = pxc.fetchCommandFirstPos(c.TAG_P_CMD_HA1, &test);
    EXPECT_EQ(6, res);

    res = pxc.fetchCommandFirstPos(c.TAG_P_COMMANDCOUNT_INT, &test);
    EXPECT_EQ(6, res);
}

#endif // XMLLISTGENERATOR_TEST_H
