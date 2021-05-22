/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef INNERSTACKEDWIDGET_H
#define INNERSTACKEDWIDGET_H

#include "searchcombobox.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QStackedWidget>
#include <QToolButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QFileDialog>

#include <editoperator.h>

class InfoStacked : public QStackedWidget
{
    Q_OBJECT
public:
    explicit InfoStacked(QWidget *parent = nullptr);
    ~InfoStacked() override;

    void setEditOperator(EditOperator *op);

    void updateIndex(QString operation);
    void reloadAction();

public slots:
    void setInfoDataList(int after, int unused1, int function);

private slots:
    void editAuthorAction(QString text);
    void editProjectNameAction(QString text);
    void editVerAction(QString text);

    void editDescriptionAction(QString text);

    void editInitialSearch(QString text);

    void editCheckAction(bool check);
    void editTextValueAction(QString value);

private:
    bool switchStackedWidget(int after, int function);

    EditOperator *editop;

    QWidget *stackwidget;
    QLineEdit *name;
    QLineEdit *ver;
    QLineEdit *author;
    QLineEdit *desc;

    QToolButton *addbutton;
    QToolButton *editbutton;
    QToolButton *deletebutton;
    QToolButton *basepathopenbutton;

    QCheckBox *finput;
    QCheckBox *sinput;
    SearchComboBox *fscombo;
    QCheckBox *rloop;
    QLineEdit *rloopmax;
    QLineEdit *rlargs;
    QLineEdit *reloop;
    QLineEdit *basepath;
    QLabel *rlabel;
};

#endif // INNERSTACKEDWIDGET_H
