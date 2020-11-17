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

#include "searchdialogtable.h"

SearchDialogTable::SearchDialogTable(QWidget *)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setPopupActionTop();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Search Result")));
    // adjust row
    resizeRowsToContents();
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set vertical header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                  , qpc.VERTICAL_HEADER_ENABLE);

    //set header sort settings
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(selectAll()));

    //set doubleclick action
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(propertyAction()));
}

SearchDialogTable::~SearchDialogTable()
{

}

void SearchDialogTable::copyAction()
{
    QClipboard *clipboard = QApplication::clipboard();

    QString bindString;
    QModelIndexList indexlist = this->selectedIndexes();
    int counter = indexlist.count();

    for(int i = 0; i < counter; i++){
        bindString.append(indexlist.at(i).data().toString());
        bindString.append("\n");
    }

    clipboard->setText(bindString);
}

void SearchDialogTable::propertyAction()
{
    if(this->rowCount() > 0){
        FileInfoDialog *dialog = new FileInfoDialog();
        dialog->setStyleSheet(this->styleSheet());
        dialog->move(this->mapToGlobal(QPoint(this->geometry().center().x(),this->geometry().top()) \
                                       - QPoint(dialog->rect().center().x(), dialog->rect().bottom())));
        dialog->setFileInfo(this->selectedItems().at(0)->text());
        dialog->show();
    }
}

void SearchDialogTable::setPopupActionTop()
{
    //init menu context
    m_copy = addTableAction(ACTION::COPY, Qt::CTRL + Qt::Key_C);
    m_property = addTableAction(ACTION::PROPERTY, Qt::CTRL + Qt::Key_P);

    // connect signals
    connect(m_copy, &QAction::triggered, this, &SearchDialogTable::copyAction);
    connect(m_property, &QAction::triggered, this, &SearchDialogTable::propertyAction);
}

bool SearchDialogTable::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Up:
             if(this->currentRow() != 0)
                 selectRow(this->currentRow() - 1);
             break;

           case Qt::Key_Down:
             if(this->rowCount() - 1 != this->currentRow())
                 selectRow(this->currentRow() + 1);
            break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 copyAction();
             break;

           case Qt::Key_P:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                propertyAction();
             break;

           default:
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
