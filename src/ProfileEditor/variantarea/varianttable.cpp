/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "varianttable.h"

VariantTable::VariantTable(QWidget *)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //init table size
    setColumnCount(2);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //hide row numbers
    verticalHeader()->setVisible(false);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("File Variant") << tr("Value")));

    //set edit saving
    connect(this, &QTableWidget::cellChanged, this, &VariantTable::textEditedAction);
}

VariantTable::~VariantTable()
{

}

void VariantTable::setEditOperator(EditOperator *op)
{
    editop = op;
}

void VariantTable::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");
    if(sep.count() < 2) return;

    int command = sep.last().toInt();

    int rowpos = -1;
    int tablerowpos = -1;

    switch (command) {
    case UiCommandMap::LV_ADD:
        this->insertItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::LV_INSERT:
        this->insertItem(static_cast<QString>(sep.at(1)).toInt());
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        this->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos));
        break;

    case UiCommandMap::LV_EDIT:
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        this->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos));
        break;

    case UiCommandMap::LV_DELETE:
        this->deleteItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::LV_SWAP:
        this->swapItem(static_cast<QString>(sep.at(0)).toInt()
                           , static_cast<QString>(sep.at(1)).toInt());
        break;

    default:
        break;
    }
}

void VariantTable::addAction()
{
    int row = this->rowCount();
    setRowCount(row + 1);
    editop->tableEditVariantAction(LOCALINDEX, row, getLocalVariants(row), UiCommandMap::LV_ADD);

    //for usability
    this->setCurrentItem(itemAt(row,0));
    this->selectRow(row);

    editAction();
}

void VariantTable::editAction()
{
    this->clearSelection();
    this->edit(currentIndex());
}

void VariantTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(!deleteCheckMessage()) return;

    QModelIndexList lists = this->selectedIndexes();

    // 2 rows
    int rows = lists.count() / 2;
    for(int i = 0; i < rows; i++){
        qDebug() << "VariantTable::deleteAction : selectrow : " << lists.at(i).row();

        editop->tableEditVariantAction(LOCALINDEX, lists.at(i).row(), \
                                       getLocalVariants(-1), UiCommandMap::LV_DELETE);
    }

    //delete file items
    this->deleteTableRecursive();
}

void VariantTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();

    int rows = mlist.count() / 2;
    for(int i = 0; i < rows; i++){
        tmp.append(this->model()->index(mlist.at(i).row(), 0).data().toString());
        tmp.append("\t");
        tmp.append(this->model()->index(mlist.at(i).row(), 1).data().toString());
        tmp.append("\n");

        editop->tableEditVariantAction(LOCALINDEX, mlist.at(i).row(), \
                                       getLocalVariants(-1), UiCommandMap::LV_DELETE);
    }

    //force delete
    BaseTable::deleteTableRecursive();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);
}

void VariantTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //item copy
    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();

    // 2 column
    int counter = mlist.count() / 2;
    for(int i = 0; i < counter; i++){
        tmp.append(this->model()->index(mlist.at(i).row(), 0).data().toString());
        tmp.append("\t");
        tmp.append(this->model()->index(mlist.at(i).row(), 1).data().toString());
        tmp.append("\n");
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);
}

void VariantTable::pasteAction()
{
    //modified CommandTable::pasteEnterAction()
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\t|\\n|\\r\\n"));

    //last lests unknown ""
    if(text.count() > 1) text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       QStringList intext = (static_cast<QString>(text.at(i))).split(QRegularExpression("\\t|,"));

       int intxt = intext.count();
       if(intxt > 0){
           this->setItem(row, 0, new QTableWidgetItem(intext.at(0)));
           if(intxt > 1){
               this->setItem(row, 1, new QTableWidgetItem(intext.at(1)));
           }
       }
       editop->tableEditVariantAction(LOCALINDEX, i, intext, UiCommandMap::LV_INSERT);
    }

    //save to backup file
//    editAction();
}

void VariantTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    tableItemSwap(current, current-1);

    selectRow(current - 1);
}

void VariantTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    tableItemSwap(current, current+1);

    selectRow(current + 1);
}

void VariantTable::reloadAction()
{
    qDebug() << "VariantTable::reloadaction";
    this->blockSignals(true);
    setRowCount(0);
    setLocalListItem(LOCALINDEX);
    this->blockSignals(false);
}

void VariantTable::textEditedAction(int row, int column)
{
    Q_UNUSED(column)

    qDebug() << "VariantTable::textEditedAction : row:" << row
             << " col:" << column;

    editop->tableEditVariantAction(LOCALINDEX, row, getLocalVariants(row), UiCommandMap::LV_EDIT);
}

void VariantTable::insertItem(int row)
{
    this->blockSignals(true);
    this->insertRow(row);
    this->setCurrentItem(itemAt(row,0));
    this->selectRow(row);
    this->blockSignals(false);
}

void VariantTable::deleteItem(int row)
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;
    this->removeRow(row);
}

void VariantTable::replaceItem(int row, QStringList strlist)
{
    this->blockSignals(true);
    this->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    this->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    this->clearSelection();
    selectRow(row);
    this->blockSignals(false);
}

void VariantTable::swapItem(int before, int after)
{
    tableItemSwap(before, after);

    this->clearSelection();
    selectRow(after);
}

void VariantTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/default_icons/edit.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &VariantTable::addAction);
    connect(m_delete, &QAction::triggered, this, &VariantTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &VariantTable::editAction);
}

void VariantTable::setPopupActionDefault()
{
    //set basic items
    m_cut = contextMenu->addAction(QIcon(":/default_icons/cut.png"), tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/default_icons/paste.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    contextMenu->addSeparator();

    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_cut, &QAction::triggered, this, &VariantTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &VariantTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &VariantTable::pasteAction);
    connect(m_up, &QAction::triggered, this, &VariantTable::upAction);
    connect(m_down, &QAction::triggered, this, &VariantTable::downAction);
}

void VariantTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, &QAction::triggered, this, &VariantTable::reloadAction);
}

bool VariantTable::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    auto mdCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::AltModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:  if (mdCheck()) addAction();    break;
        case Qt::Key_Delete: if (mdCheck()) deleteAction(); break;
        case Qt::Key_E:      if (mdCheck()) editAction();   break;

        case Qt::Key_Up:
            if(mdCheck()){
                upAction();
            }else{
                if(this->currentRow() != 0)
                    selectRow(this->currentRow() - 1);
            }
            break;

        case Qt::Key_Down:
            if(mdCheck()){
                downAction();
            }else{
                if(this->rowCount() - 1 != this->currentRow())
                    selectRow(this->currentRow() + 1);
            }
            break;

        case Qt::Key_X:  if(mdCheck()) cutAction();    break;
        case Qt::Key_C:  if(mdCheck()) copyAction();   break;
        case Qt::Key_V:  if(mdCheck()) pasteAction();  break;
        case Qt::Key_R:  if(mdCheck()) reloadAction(); break;

        default:
            //qDebug("Ate key press %d", keyEvent->key());
            break;
        }
        return true;
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}

//DEPENDS_XML OUTPUT PROCESS
bool VariantTable::setLocalListItem(int itemid)
{
    EditorCache list;

    if(!editop->read(itemid, &list)){
        return false;
    }

    FunctionType fs;

    //get type
    QString type = list.type;

    //set root
    if(fs.getType(list.type) != fs.TYPE::LOCAL){
        return false;
    }

    int counter = list.local.variantData.count();
    setRowCount(counter);

    for(int i = 0; i < counter; i++){
        VariantPair pair = list.local.variantData.at(i);
        this->setItem(i, 0, new QTableWidgetItem(pair.variant));
        this->setItem(i, 1, new QTableWidgetItem(pair.value));
    }

    qDebug()<< "[VariantTable::setLocalListItem] success";
    return true;
}

void VariantTable::tableItemSwap(int from, int to)
{
    QString bku0 = this->model()->index(from, 0).data().toString();
    QString bku1 = this->model()->index(from, 1).data().toString();

    //set tableitem
    this->setItem(from, 0, new QTableWidgetItem(this->model()->index(to, 0).data().toString()));
    this->setItem(from, 1, new QTableWidgetItem(this->model()->index(to, 1).data().toString()));
    //set tableitem2
    this->setItem(to, 0, new QTableWidgetItem(bku0));
    this->setItem(to, 1, new QTableWidgetItem(bku1));
}

QStringList VariantTable::getTableData(int targetrow, int tablerow)
{
    EditorCache list;

    if(targetrow != LOCALINDEX || !editop->read(targetrow, &list)){
        return QStringList() << "" << "";
    }

    VariantPair pair = list.local.variantData.at(tablerow);

    return QStringList() <<  pair.variant << pair.value;

}

QStringList VariantTable::getLocalVariants(int index)
{
    QStringList list;

    if(index > -1 && index < this->rowCount()){
        list.append(this->model()->index(index, 0).data().toString());
        list.append(this->model()->index(index, 1).data().toString());
    }else{
        list.append("");
        list.append("");
    }

    return list;
}
