#include "commandtable.h"

CommandTable::CommandTable(QWidget *)
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
}

void CommandTable::editAction()
{
    this->edit(currentIndex());
}

void CommandTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
        this->deleteTableRecursive();
}

void CommandTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    int current = this->currentRow();
    QString tmp = this->model()->index(current, 0).data().toString();
    this->insertRow(current);
    this->setRowCount(this->rowCount() + 1);
    this->setItem(current, 0, new QTableWidgetItem(tmp));
    selectRow(current + 1);
}

void CommandTable::pasteAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(" ");
    int row = this->rowCount();
    int txcount = text.count();
    setRowCount(row + txcount);

    for(int i = 0; i < txcount; i++){
        this->setItem(row + i, 0, new QTableWidgetItem(text.at(i)));
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

    selectRow(current + 1);
}

void CommandTable::openFileAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFile("./");
    //this->in
    this->setItem(current, 0, new QTableWidgetItem(str));
}

void CommandTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder("./");
    this->setItem(current, 0, new QTableWidgetItem(str));
}

void CommandTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/icons/Add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/icons/Denided.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/icons/Pen.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();
    m_file = contextMenu->addAction(QIcon(":/icons/File.png"), tr("Select file..."));
    m_dir = contextMenu->addAction(QIcon(":/icons/Folder.png"), tr("Select Dir..."));
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
    m_copy = contextMenu->addAction(QIcon(":/icons/Files_Copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/icons/Clipboard_Full.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    m_up = contextMenu->addAction(QIcon(":/icons/Button_Up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/icons/Button_Down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_copy, SIGNAL(triggered()), this, SLOT(copyAction()));
    connect(m_paste, SIGNAL(triggered()), this, SLOT(pasteAction()));
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
             if (keyEvent->modifiers() & Qt::ControlModifier)
               addAction();
             break;

           case Qt::Key_Delete:
             deleteAction();
             break;

           case Qt::Key_Up:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 copyAction();
             break;

           case Qt::Key_E:
             if (keyEvent->modifiers() & Qt::ControlModifier)
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
