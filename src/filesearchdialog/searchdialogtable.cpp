#include "searchdialogtable.h"

SearchDialogTable::SearchDialogTable(QWidget *)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //init menu context
    m_copy = contextMenu->addAction(tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    m_property = contextMenu->addAction(tr("Property"));
    m_property->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));

    // connect signals
    connect(m_copy, SIGNAL(triggered()), this, SLOT(copyAction()));
    connect(m_property, SIGNAL(triggered()), this, SLOT(propertyAction()));

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Search Result")));
    // adjust row
    resizeRowsToContents();
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

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
        bindString.append(tr("\n"));
    }

    clipboard->setText(bindString);
}

void SearchDialogTable::propertyAction()
{
    if(this->rowCount() > 0){
        FileInfoDialog *dialog = new FileInfoDialog();
        dialog->setFileInfo(this->selectedItems().at(0)->text());
        dialog->show();
    }
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
