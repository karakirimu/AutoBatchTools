/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILECOMBOBOX_H
#define PROFILECOMBOBOX_H

#include "profilecombobox_global.h"

#include <QComboBox>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <../profilexmlbuilder/profilexmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>

class PROFILECOMBOBOXSHARED_EXPORT ProfileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ProfileComboBox(QObject *parent = nullptr);
    ~ProfileComboBox();

    QString getCurrentFileName();
    QString getCurrentFileName(int index);
//    int getIndexFromFileName(QString filepath);

    void setIndex(QString filepath);

public slots:
    void reloadComboBoxItem();
    void addItemAction();
    void deleteItemAction();

private:
    ProfileXmlBuilder *builder;
    ProcessXmlBuilder *pbuilder;

};

#endif // PROFILECOMBOBOX_H
