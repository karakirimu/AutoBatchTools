/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBBASICTABLE_H
#define LIBBASICTABLE_H

#include "basictable_global.h"
#include "basetable.h"
#include "tablemimedata.h"
#include <QClipboard>
#include <QHeaderView>
#include <QMessageBox>

class BASICTABLESHARED_EXPORT BasicTable : public BaseTable
{
    Q_OBJECT
public:
    explicit BasicTable(QWidget *parent = nullptr);
    virtual ~BasicTable() override;

protected slots:
    virtual void editAction()   { return; }
    virtual void addAction()    { return; }
    virtual void deleteAction() { return; }
    virtual void copyAction()   { return; }
    virtual void upAction();
    virtual void downAction();

    virtual void horizontalHeaderClicked(int cols);

protected:
    void installClipboardFilter(QObject *caller);
    void setPopupActionDefault(QIcon copy, QIcon up, QIcon down);

    void upSelected(std::function<void (int)> postup);
    void downSelected(std::function<void(int)> postdown);

    void copyToClipboard(QClipboard *clipboard);
    const QList<QStringList> tableSelectionToList();

    bool pasteFromClipboard(QClipboard *clipboard);
    bool insertItemFromList(const QList<QStringList> &list);

    enum class ACTION{
                NEWFILE, ADD, REMOVE, EDIT, CLEAR,
                ENABLE, DISABLE, FILE, FILES, FOLDER, FILEINFO,
                CUT, COPY, PASTE, PASTESPACE, PASTENEWLINE, UP, DOWN,
                REFRESH, PROPERTY};

    QIcon getIcon(BasicTable::ACTION action);
    QString getActionText(BasicTable::ACTION action);

    bool deleteCheckMessage();
    QString selectFile(QString basedir);
    QString selectFolder(QString basedir);

    QAction * addTableAction(BasicTable::ACTION action,
                            int keycode1 = 0,
                            int keycode2 = 0,
                            int keycode3 = 0,
                            int keycode4 = 0);

    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;
    QString mimetypeclass;
};

#endif // LIBBASICTABLE_H
