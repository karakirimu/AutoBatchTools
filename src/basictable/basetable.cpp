#include "basetable.h"

BaseTable::BaseTable(QWidget *)
{
    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    //init menu context
    contextMenu = new QMenu(this);
}

BaseTable::~BaseTable(){
    delete contextMenu;
}

void BaseTable::dropEvent(QDropEvent *)
{
    return;
}

void BaseTable::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

QStringList BaseTable::droppedFromOutside(QDropEvent *event)
{
    QStringList droppeddata;

    //QUrlに取得した複数URLを1つずつ代入する。
    foreach(QUrl url, event->mimeData()->urls()){
        droppeddata.append(url.toLocalFile());
    }

    event->acceptProposedAction();
    return droppeddata;
}

void BaseTable::droppedFromInside(QDropEvent *event)
{
    insertTableRow(this->row(this->selectedItems().at(0))
                   ,this->indexAt(event->pos()).row());
}

void BaseTable::swapTableRow(int from, int dest)
{
    QTableWidgetItem *temp;
    for (int col = 0; col < this->columnCount(); col++){
        temp = this->takeItem(from, col);
        this->setItem(from, col, this->takeItem(dest, col));
        this->setItem(dest, col, temp);
    }
}

void BaseTable::insertTableRow(int from, int dest)
{
    int distance = dest - from;
    QTableWidgetItem *temp;

    this->insertRow(dest);

    // + 1 means effect of insertrow
    from = from + ((distance > 0)? 0 : 1);

    for (int col = 0; col < this->columnCount(); col++){
        temp = this->takeItem(from, col);
        this->setItem(dest, col, temp);
    }

    this->removeRow(from);
}

void BaseTable::copyTable(int index){

    this->setRowCount(this->rowCount() + 1);

    for (int col = 0; col < this->columnCount(); col++){

        for(int i = this->rowCount(); i > index; i--){
            //copy action
            this->setItem(i, col, this->takeItem(i-1, col));
        }
    }
}

void BaseTable::deleteTableRecursive()
{
//　　　　itemがあるときだけしか消えない
//    QList <QTableWidgetItem *> items;
//    for (int x = 0; x < this->rowCount(); ++x)
//    {
//      items = this->selectedItems();
//      if(!items.empty()){
//          this->removeRow(items.at(0)->row());
//          delete items.at(0);
//      }
//    }

//    QModelIndexList lists;
//    for (int x = 0; x < this->rowCount(); ++x)
//    {
//        lists = this->selectedIndexes();
//        if(!lists.empty()){
//          this->removeRow(lists.at(0).row());

//          //delete column index
//          for(int i = 0; i < lists.at(0).column(); i++){
//              lists.removeAt(0);
//          }
//        }
//    }
    //すべての条件で消える
    QModelIndexList lists = this->selectedIndexes();

    while(!lists.empty()){
        this->removeRow(lists.at(0).row());

        //delete column index
        for(int i = 0; i < lists.at(0).column(); i++){
            lists.removeAt(0);
        }
        lists = this->selectedIndexes();
    }
}

///
/// \fn BaseTable::eventFilter
/// \brief grab key event
/// \param obj
/// \param event
/// \return
///
bool BaseTable::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << event->type();
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             qDebug() << "Enter";
             break;
           case Qt::Key_Escape:
             qDebug() << "Escape";
             break;
           case Qt::Key_Insert:
             qDebug() << "Insert";
             break;
           case Qt::Key_Delete:
             qDebug() << "Delete";
             deleteTableRecursive();
             break;
           default:
             qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void BaseTable::onCustomContextMenu(const QPoint &point)
{
    //    QModelIndex index = this->indexAt(point);
    //    if (index.isValid() && index.row() % 2 == 0) {
    //      // contextMenu->exec(this->mapToGlobal(point));
    //    }

    //    qDebug() << index;
    contextMenu->popup(mapToGlobal(point));
}

QStringList BaseTable::selectFiles(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    return dialog.getOpenFileNames(this, tr("Open Files"), basedir, tr("Files (*.*)"));
}

QString BaseTable::selectFile(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    return dialog.getOpenFileName(this, tr("Open File"), basedir, tr("File (*.*)"));
}

QString BaseTable::selectFolder(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    return dialog.getExistingDirectory(this, tr("Open Folder"), basedir);
}
