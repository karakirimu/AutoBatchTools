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

#ifndef CONSOLEBASE_H
#define CONSOLEBASE_H

#include "consolebase_global.h"
#include <QWidget>
#include <QTextEdit>
#include <QThread>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <../executor/executor.h>

#define RANGE 30

class CONSOLEBASESHARED_EXPORT ConsoleBase : public QTextEdit
{
    Q_OBJECT
public:
    explicit ConsoleBase(QWidget *parent = nullptr);
    ~ConsoleBase();

protected slots:
    void started(int runtype);
    void pause();
    void stop();
    void end(int runtype);

    void updateText(QString data, int type);
    void updateExternalError(QString str);

    //context menu
    void onCustomContextMenu(const QPoint &point);
    void setPopupAction();

    //move position
    void moveCursorToEnd();

private:
    bool eventFilter(QObject *obj, QEvent *event);

    //for send message to executor
    QString inputstr;

    QMenu *contextMenu;
    QAction *m_selectall;
    QAction *m_copy;
    QAction *m_clear;
};

#endif // CONSOLEBASE_H
