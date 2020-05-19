/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

#include <editoperator.h>

class InfoStacked : public QStackedWidget
{
public:
    explicit InfoStacked(QWidget *parent = nullptr);
    ~InfoStacked();

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

    QCheckBox *finput;
    QCheckBox *sinput;
    SearchComboBox *fscombo;
    QCheckBox *rloop;
    QLineEdit *rloopmax;
    QLineEdit *rlargs;
    QLineEdit *reloop;
    QLabel *rlabel;
};

#endif // INNERSTACKEDWIDGET_H
