/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef FLOWTABLE_H
#define FLOWTABLE_H

#include <../basictable/basictable.h>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <editoperator.h>
#include <qsspropertyconstant.h>
#include "typeicondelegate.h"

class ProcessFlowTable : public BasicTable
{
    Q_OBJECT
public:
    explicit ProcessFlowTable(QWidget *parent = nullptr);
    ~ProcessFlowTable();

    void setEditOperator(EditOperator *op);
    void updateIndex(QString operation);
    void reloadAction();

protected slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    bool eventFilter(QObject *obj, QEvent *event);

    //for data action
    void addAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();

    //for inner item set
    //syncronize with Item
    void addItem();
    void deleteItem(int id);
    void insertItem(int id);
    void swapItem(int before, int after);
    void replaceItem(int id);

    //table selection changed
    void selectChanged(int crow, int ccol, int prow, int pcol);

    void onItemStatusChanged(int after, int before, int function);

private:
    int uiIndexToData();
    inline int dataToUiIndex(int id){ return (id > 1)? id - 1 : 0; }

    //from ui index to xml data
    inline int uiIndexToData(int id){ return (id > 0)? id + 1 : 0; }

    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void setAllFlowItem();

    struct TableOption {

        //! Data index number (It must be >= 0)
        int dataid = -1;

        //! Hide description setting
        bool hidedescription = false;

        //! Foreground color
        QColor foreground;

        //! Background color
        QColor background;
    };

    void setFlowItem(int dataid);
    void setAllIncludeItem(EditorCache *list, TableOption *option);
    void setInfoItem(EditorCache *list, TableOption *option);
    void setExecuteItem(EditorCache *list, TableOption *option);
    void setFileSearchItem(EditorCache *list, TableOption *option);
    void setPluginItem(EditorCache *list, TableOption *option);
    void setProfileLoadItem(EditorCache *list, TableOption *option);

    void setColorSetting(const QString background
                         , const QString defaultbackground
                         , const QString textcolor
                         , const QString defaulttextcolor
                         , TableOption *option);
    void setSecondColumn(const QPixmap &icon, const QString &text \
                                            , const TableOption *option);

    QAction *m_add;
    QAction *m_delete;
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_ref;

    EditOperator *editop;

    enum{FIRST, SECOND};

    //for reduce read time
//    const QString INFO_TITLE      = "Information";
    const QPixmap INFO_PIXMAP     = QIcon(":/default_icons/info.png").pixmap(16,16);
//    const QString INFO_BACKGROUND = "#f8f8f8";
//    const QString INFO_FOREGROUND = "#000000";

//    const QString EXEC_TITLE      = "Executable";
    const QPixmap EXEC_PIXMAP     = QIcon(":/default_icons/terminal.png").pixmap(16,16);
//    const QString EXEC_BACKGROUND = "#e2f6ff";
//    const QString EXEC_FOREGROUND = "#000000";

//    const QString SEARCH_TITLE      = "FileSearch";
    const QPixmap SEARCH_PIXMAP     = QIcon(":/default_icons/search.png").pixmap(16,16);
//    const QString SEARCH_BACKGROUND = "#dcedc8";
//    const QString SEARCH_FOREGROUND = "#000000";

//    const QString PLUGIN_TITLE      = "Plugin";
    const QPixmap PLUGIN_PIXMAP     = QIcon(":/default_icons/extras.png").pixmap(16,16);
//    const QString PLUGIN_BACKGROUND = "#ffcdd2";
//    const QString PLUGIN_FOREGROUND = "#000000";

//    const QString PROFILELOAD_TITLE      = "ProfileLoad";
    const QPixmap PROFILELOAD_PIXMAP     = QIcon(":/default_icons/others.png").pixmap(16,16);
//    const QString PROFILELOAD_BACKGROUND = "#ffecb3";
//    const QString PROFILELOAD_FOREGROUND = "#000000";

    SettingConstant sc;
    QssPropertyConstant qpc;
};

#endif // FLOWTABLE_H
