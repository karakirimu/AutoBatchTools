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

#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#include <QApplication>
#include <QClipboard>
#include <uicommandmap.h>
#include <../basictable/basictable.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>

class CommandTable : public BasicTable
{
    Q_OBJECT
public:
    explicit CommandTable(QWidget *parent = nullptr);
    ~CommandTable();

    QString getText(int row);

    // undo redo no emit operation
    void insertItem(int row);
    void deleteItem(int row);
    void replaceItem(int row, QString str);
    void swapItem(int before, int after);
    void moveItem(int before, int beforecount, int after);

signals:
    void updateTable(int index, QString newstr, int function);
    void swapTable(int before, int after);
    void dragDropTable(QList<int> before, int after);

protected slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    virtual void addAction();
    virtual void deleteAction();
    virtual void cutAction();
    virtual void pasteAction();
    virtual void pasteSpaceAction();
    virtual void pasteEnterAction();

    virtual void editedAction(int row, int column);

private slots:
    void editAction();
    void copyAction();
    void upAction();
    void downAction();
    void openFileAction();
    void openDirectoryAction();

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_pastespace;
    QAction *m_pasteenter;
};

#endif // COMMANDTABLE_H
