/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#include <QApplication>
#include <QClipboard>
#include <uicommandmap.h>
#include <../basictable/basictable.h>
#include <../share/qss/qsspropertyconstant.h>

class CommandTable : public BasicTable
{
    Q_OBJECT
public:
    explicit CommandTable(QWidget *parent = nullptr);
    ~CommandTable() override;

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
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    virtual void addAction() override;
    virtual void deleteAction() override;
    virtual void cutAction();
    virtual void pasteAction();
    virtual void pasteSpaceAction();
    virtual void pasteEnterAction();

    virtual void editedAction(int row, int column);

private slots:
    void editAction() override;
    void copyAction() override;
    void upAction() override;
    void downAction() override;
    void openFileAction();
    void openDirectoryAction();

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

    void setPopupActionTop();
    void setPopupActionDefault();
    void paste(const QRegularExpression &exp);

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
