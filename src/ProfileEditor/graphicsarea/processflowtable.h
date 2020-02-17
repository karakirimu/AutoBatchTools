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

#ifndef FLOWTABLE_H
#define FLOWTABLE_H

#include <../basictable/basictable.h>
#include <editoperator.h>
//#include <graphicsarea/flowcellwidget.h>

class ProcessFlowTable : public BasicTable
{
    Q_OBJECT
public:
    explicit ProcessFlowTable(QWidget *parent = nullptr);
    ~ProcessFlowTable();

    void setEditOperator(EditOperator *op);

    void reloadAction();

    void updateIndex(QString operation);

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

    void onItemStatusChanged(int after, int before, int function, int sendfrom);

private:
    // argument is ui index
//    void excludeSelector(int crow, int ccol, int prow, int pcol);

//    void mousePressEvent(QMouseEvent *event);
    int fixedCurrentRow();
    inline int dataToUiIndex(int id);

    //from ui index to xml data
    inline int uiIndexToData(int id);

    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void setFlowItem(int itemid);
    void setAllFlowItem();
    void setTempItem(QList<QStringList> *list, int dataid);
    void setInfoItem(QList<QStringList> *list, int dataid);
    void setNormalItem(QList<QStringList> *list, int dataid);
    void setSearchItem(QList<QStringList> *list, int dataid);
    void setPluginsItem(QList<QStringList> *list, int dataid);
    void setProfileItem(QList<QStringList> *list, int dataid);

//    void updateLastIndexItem(int lastindex);

    QAction *m_add;
    QAction *m_delete;
    QAction *m_cut;
//    QAction *m_copy;
    QAction *m_paste;
//    QAction *m_up;
//    QAction *m_down;
    QAction *m_ref;

    EditOperator *editop;
    ProcessXmlListGenerator pxlg;

    enum{FIRST, SECOND};

    //for reduce read time
    const QString info_title = "Information";
    const QPixmap info_pixmap = QIcon(":/default_icons/info.png").pixmap(16,16);
    const QString info_style = "color: black; background-color: #e8e8e8;";
    const QString info_frame = "background-color: #606060;";

    const QString exec_title = "Executable";
    const QPixmap exec_pixmap = QIcon(":/default_icons/terminal.png").pixmap(16,16);
    const QString exec_style = "color: black; background-color: #e2f6ff;";
    const QString exec_frame = "background-color: #b0c3cc;";

    const QString search_title = "FileSearch";
    const QPixmap search_pixmap = QIcon(":/default_icons/search.png").pixmap(16,16);
    const QString search_style = "color: black; background-color: #dcedc8;";
    const QString search_frame = "background-color: #aabb97;";

    const QString plugin_title = "Plugins";
    const QPixmap plugin_pixmap = QIcon(":/default_icons/extras.png").pixmap(16,16);
    const QString plugin_style = "color: black; background-color: #ffcdd2;";
    const QString plugin_frame = "background-color: #cb9ca1;";

    const QString other_title = "ProjectLoad";
    const QPixmap other_pixmap = QIcon(":/default_icons/others.png").pixmap(16,16);
    const QString other_style = "color: black; background-color: #ffecb3;";
    const QString other_frame = "background-color: #cbba83;";
};

#endif // FLOWTABLE_H
