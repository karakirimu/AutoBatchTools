#include "commandtable.h"

CommandTable::CommandTable(QWidget *parent)
    : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Runnable file / Arguments")));

    connect(this,&CommandTable::cellChanged, this, &CommandTable::editedAction);
}

CommandTable::~CommandTable()
{

}

QString CommandTable::getText(int row)
{
    return this->model()->index(row, 0).data().toString();
}

void CommandTable::addAction()
{
    int row = this->rowCount();
    setRowCount(row + 1);
    emit updateTable(row, "", ProcessXmlListGenerator::TABLE_ADD);

    //for useability
    this->setCurrentItem(itemAt(row,0));
    this->selectRow(row);

    editAction();
}

void CommandTable::editAction()
{
    this->clearSelection();
    this->edit(currentIndex());
}

void CommandTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage()){
        QModelIndexList lists = this->selectedIndexes();
        int rows = lists.count();
        for(int i = 0; i < rows; i++){
            emit updateTable(lists.at(i).row(), "", ProcessXmlListGenerator::TABLE_DELETE);
        }
        BaseTable::deleteTableRecursive();
    }
}

//FIXME : multiple select (it msy be not so good ...)
void CommandTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();
    int rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");

        emit updateTable(mlist.at(i).row(), "", ProcessXmlListGenerator::TABLE_DELETE);
    }

    //force delete
    BaseTable::deleteTableRecursive();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);
}

void CommandTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

//    int current = this->currentRow();
//    QString tmp = this->model()->index(current, 0).data().toString();
//    this->insertRow(current);
//    this->setRowCount(this->rowCount() + 1);
//    this->setItem(current, 0, new QTableWidgetItem(tmp));
//    selectRow(current + 1);
//    QClipboard *clipboard = QApplication::clipboard();
//    clipboard->setText(tmp);

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();
    int rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);

}

void CommandTable::pasteAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split("\t");

    //last lests unknown ""
    if(text.count() > 1) text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
       emit updateTable(row, text.at(i), ProcessXmlListGenerator::TABLE_INSERT);
    }
}

void CommandTable::pasteSpaceAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\t| "));

    //last lests unknown ""
    text.removeLast();
    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
       emit updateTable(row, text.at(i), ProcessXmlListGenerator::TABLE_INSERT);
    }
}

void CommandTable::pasteEnterAction()
{
    QClipboard *clipboard = QApplication::clipboard();

    QStringList text = clipboard->text().split(QRegularExpression("\\t|\\n|\\r\\n"));

    //last lests unknown ""
    text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
        if(text.at(i) != "\n" || text.at(i) != "\r\n"){
            if(row > 0) row = this->currentRow();
            insertRow(row);
            this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
            emit updateTable(row, text.at(i), ProcessXmlListGenerator::TABLE_INSERT);
        }
    }
}

void CommandTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    //swap item
    QString tmp = this->model()->index(current - 1, 0).data().toString();
    this->setItem(current - 1, 0, new QTableWidgetItem(this->model()->index(current, 0).data().toString()));
    this->setItem(current, 0, new QTableWidgetItem(tmp));

    emit swapTable(current, current - 1);
    this->clearSelection();
    selectRow(current - 1);
}

void CommandTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    //swap item
    QString tmp = this->model()->index(current + 1, 0).data().toString();
    this->setItem(current + 1, 0, new QTableWidgetItem(this->model()->index(current, 0).data().toString()));
    this->setItem(current, 0, new QTableWidgetItem(tmp));

    emit swapTable(current, current + 1);
    this->clearSelection();
    selectRow(current + 1);
}

void CommandTable::openFileAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFile("./");
    //this->in
    this->setItem(current, 0, new QTableWidgetItem(str));
    if(str != ""){
        editedAction(current, 0);
    }
}

void CommandTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder("./");
    this->setItem(current, 0, new QTableWidgetItem(str));
    if(str != ""){
        editedAction(current, 0);
    }
}

void CommandTable::editedAction(int row, int column)
{
    qDebug() << "CommandTable : editedAction";
     emit updateTable(row, this->item(row, column)->text(), ProcessXmlListGenerator::TABLE_EDIT);
}

void CommandTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/default_icons/edit.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_E));
    contextMenu->addSeparator();
    m_file = contextMenu->addAction(QIcon(":/default_icons/file.png"), tr("Select file..."));
    m_dir = contextMenu->addAction(QIcon(":/default_icons/folder.png"), tr("Select Dir..."));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));

    connect(m_file, SIGNAL(triggered()), this, SLOT(openFileAction()));
    connect(m_dir, SIGNAL(triggered()), this, SLOT(openDirectoryAction()));
}

void CommandTable::setPopupActionDefault()
{
    //set basic items
    m_cut = contextMenu->addAction(QIcon(":/default_icons/cut.png"), tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::ALT + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));

    contextMenu->addSeparator();

    m_paste = contextMenu->addAction(QIcon(":/default_icons/paste.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::ALT + Qt::Key_V));

    m_pastespace = contextMenu->addAction(tr("Paste separated by space"));
    m_pasteenter = contextMenu->addAction(tr("Paste separated by newline"));

    contextMenu->addSeparator();

    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Down));

    //connect signals
    connect(m_cut, SIGNAL(triggered()), this, SLOT(cutAction()));
    connect(m_copy, SIGNAL(triggered()), this, SLOT(copyAction()));
    connect(m_paste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    connect(m_pastespace, SIGNAL(triggered()), this, SLOT(pasteSpaceAction()));
    connect(m_pasteenter, SIGNAL(triggered()), this, SLOT(pasteEnterAction()));
    connect(m_up, SIGNAL(triggered()), this, SLOT(upAction()));
    connect(m_down, SIGNAL(triggered()), this, SLOT(downAction()));
}

bool CommandTable::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             if (keyEvent->modifiers() & Qt::AltModifier)
               addAction();
             break;

           case Qt::Key_Delete:
             if (keyEvent->modifiers() & Qt::AltModifier)
               deleteAction();
             break;

           case Qt::Key_Up:
             if (keyEvent->modifiers() & Qt::AltModifier){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::AltModifier){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_X:
             if (keyEvent->modifiers() & Qt::AltModifier)
                 cutAction();
             break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::AltModifier)
                 copyAction();
             break;

           case Qt::Key_V:
             if (keyEvent->modifiers() & Qt::AltModifier)
                 pasteAction();
             break;

           case Qt::Key_E:
             if (keyEvent->modifiers() & Qt::AltModifier)
                 editAction();
             break;

           default:
             //qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
