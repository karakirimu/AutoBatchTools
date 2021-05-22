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
#include <QHeaderView>
#include <QMessageBox>

class BASICTABLESHARED_EXPORT BasicTable : public BaseTable
{
    Q_OBJECT
public:
    explicit BasicTable(QWidget *parent = nullptr);
    ~BasicTable();

protected slots:
    virtual void editAction();
    virtual void addAction();
    virtual void deleteAction();
    virtual void copyAction();
    virtual void upAction();
    virtual void downAction();

    void horizontalHeaderClicked(int cols);

protected:
    virtual void setPopupActionTop();
    virtual void setPopupActionBottom();
    void setPopupActionDefault(QIcon copy, QIcon up, QIcon down);
    bool deleteCheckMessage();

    enum class ACTION{
                NEWFILE, ADD, REMOVE, EDIT, CLEAR,
                ENABLE, DISABLE, FILE, FILES, FOLDER, FILEINFO,
                CUT, COPY, PASTE, PASTESPACE, PASTENEWLINE, UP, DOWN,
                REFRESH, PROPERTY};

    QIcon getIcon(BasicTable::ACTION action);
    QString getActionText(BasicTable::ACTION action);

    QAction * addTableAction(BasicTable::ACTION action,
                            int keycode1 = 0,
                            int keycode2 = 0,
                            int keycode3 = 0,
                            int keycode4 = 0);

    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;

private:

};

#endif // LIBBASICTABLE_H
