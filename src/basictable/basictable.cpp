/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "basictable.h"


BasicTable::BasicTable(QWidget *parent)
    : BaseTable(parent)
{
    //set header sort settings
    connect(horizontalHeader()
            , &QHeaderView::sectionClicked
            , [=](int logicalIndex){ horizontalHeaderClicked(logicalIndex); });
}

BasicTable::~BasicTable(){

}

void BasicTable::setPopupActionDefault(QIcon copy, QIcon up, QIcon down)
{
    //set basic items
    m_copy = contextMenu->addAction(copy, tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_C));

    m_up = contextMenu->addAction(up, tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_Up));

    m_down = contextMenu->addAction(down, tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_Down));

    //connect signals
    connect(m_copy, &QAction::triggered, this, &BasicTable::copyAction);
    connect(m_up, &QAction::triggered, this, &BasicTable::upAction);
    connect(m_down, &QAction::triggered, this, &BasicTable::downAction);
}

bool BasicTable::deleteCheckMessage()
{
    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
        this
        , tr("Alert")
        , tr("Do you want to delete selected item(s)?")
        , QMessageBox::Yes | QMessageBox::No );

    return (res == QMessageBox::Yes);
}

/**
 * @fn BasicTable::selectFile
 * @brief Selects a file and returns a string.
 * @param basedir Folder to display when a window is opened.
 * @return Path of the selected file.
 */
QString BasicTable::selectFile(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);

    return dialog.getOpenFileName(this, tr("Open File"), basedir, tr("File (*.*)"));
}

/**
 * @fn BasicTable::selectFolder
 * @brief Selects a folder and returns a string.
 * @param basedir Folder to display when a window is opened.
 * @return Path of the selected folder.
 */
QString BasicTable::selectFolder(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);

    return dialog.getExistingDirectory(this, tr("Open Folder"), basedir);
}

/**
 * @fn BasicTable::addTableAction
 *
 * @brief A wrapper function to simplify action creation.
 *        The icon path and the corresponding text are all
 *        defined in the class.
 *
 * @param action "enum ACTION" defined in this class
 * @param keycode1 QKeySequence keycode1 (optional)
 * @param keycode2 QKeySequence keycode2 (optional)
 * @param keycode3 QKeySequence keycode3 (optional)
 * @param keycode4 QKeySequence keycode4 (optional)
 *
 * @return created action
 */
QAction *BasicTable::addTableAction(ACTION action,
                                    int keycode1,
                                    int keycode2,
                                    int keycode3,
                                    int keycode4)
{
    QAction *res
        = contextMenu->addAction(getIcon(action), getActionText(action));
    if(keycode1 != 0){
        res->setShortcutVisibleInContextMenu(true);
        res->setShortcut(QKeySequence(keycode1,
                                      keycode2,
                                      keycode3,
                                      keycode4)
                         );
    }
    return res;
}

/**
 * @fn BasicTable::getIcon
 * @brief Default icon path definition
 * @param action "enum ACTION" defined in this class
 * @return QIcon with path assigned
 */
QIcon BasicTable::getIcon(ACTION action)
{
    QIcon icon;
    QString basename = ":/default_icons/";

    switch (action) {
    case ACTION::NEWFILE:       icon.addFile(basename + "newfile.png");    break;
    case ACTION::ADD:           icon.addFile(basename + "add.png");        break;
    case ACTION::REMOVE:        icon.addFile(basename + "remove.png");     break;
    case ACTION::EDIT:          icon.addFile(basename + "edit.png");       break;
    case ACTION::ENABLE:        icon.addFile(basename + "enable.png");     break;
    case ACTION::DISABLE:       icon.addFile(basename + "error.png");      break;
    case ACTION::CLEAR:         icon.addFile(basename + "blank.png");      break;
    case ACTION::FILE:
    case ACTION::FILES:         icon.addFile(basename + "file.png");       break;
    case ACTION::FOLDER:        icon.addFile(basename + "folder.png");     break;
    case ACTION::FILEINFO:      icon.addFile(basename + "info.png");       break;
    case ACTION::CUT:           icon.addFile(basename + "cut.png");        break;
    case ACTION::COPY:          icon.addFile(basename + "copy.png");       break;
    case ACTION::PASTE:         icon.addFile(basename + "paste.png");      break;
    case ACTION::PASTESPACE:    icon.addFile(basename + "blank.png");      break;
    case ACTION::PASTENEWLINE:  icon.addFile(basename + "blank.png");      break;
    case ACTION::UP:            icon.addFile(basename + "arrow_up.png");   break;
    case ACTION::DOWN:          icon.addFile(basename + "arrow_down.png"); break;
    case ACTION::REFRESH:       icon.addFile(basename + "refresh.png");    break;
    case ACTION::PROPERTY:      icon.addFile(basename + "blank.png");      break;
    }


    return icon;
}

/**
 * @fn BasicTable::getActionText
 * @brief Define default action text
 * @param action "enum ACTION" defined in this class
 * @return The text corresponding to the action
 */
QString BasicTable::getActionText(ACTION action)
{
    QString text = "undefined";

    switch (action) {
    case ACTION::NEWFILE:       text = tr("New");                       break;
    case ACTION::ADD:           text = tr("Add");                       break;
    case ACTION::REMOVE:        text = tr("Delete");                    break;
    case ACTION::EDIT:          text = tr("Edit");                      break;
    case ACTION::ENABLE:        text = tr("Enable");                    break;
    case ACTION::DISABLE:       text = tr("Disable");                   break;
    case ACTION::CLEAR:         text = tr("Clear");                     break;
    case ACTION::FILE:          text = tr("Add File");                  break;
    case ACTION::FILES:         text = tr("Add Files");                 break;
    case ACTION::FOLDER:        text = tr("Add Folder");                break;
    case ACTION::FILEINFO:      text = tr("File Info");                 break;
    case ACTION::CUT:           text = tr("Cut");                       break;
    case ACTION::COPY:          text = tr("Copy");                      break;
    case ACTION::PASTE:         text = tr("Paste");                     break;
    case ACTION::PASTESPACE:    text = tr("Paste (space separated)");   break;
    case ACTION::PASTENEWLINE:  text = tr("Paste (newline separated)"); break;
    case ACTION::UP:            text = tr("Up");                        break;
    case ACTION::DOWN:          text = tr("Down");                      break;
    case ACTION::REFRESH:       text = tr("Reload");                    break;
    case ACTION::PROPERTY:      text = tr("Property");                  break;
    }

    return text;
}

//void BasicTable::copyAction()
//{
//    if(this->rowCount() == 0) return;
//    copyTable(currentRow());
//}

void BasicTable::upAction()
{
    upSelected([](int row){ Q_UNUSED(row) });
}

void BasicTable::downAction()
{
    downSelected([](int row){ Q_UNUSED(row) });
}

void BasicTable::horizontalHeaderClicked(int cols)
{
    Q_UNUSED(cols)
    this->selectAll();
}

void BasicTable::installClipboardFilter(QObject *caller)
{
    mimetypeclass = QString(caller->metaObject()->className());
    qDebug() << "[BasicTable::installClipboardFilter] classname : "
             << mimetypeclass;
}

/**
 * @fn BasicTable::upSelected
 * @brief Moves the row of the selected table up one position.
 * @param postup
 * Processing after a successful "up" operation in a table.
 * The argument will be the line number before the "up" operation.
 */
void BasicTable::upSelected(std::function<void (int)> postup)
{
    if(this->rowCount() == 0
        || this->selectedItems().count() == 0) return;
    int row = this->row(this->selectedItems().at(0));
    if(row - 1 == -1) return;
    swapTableRow(row,row - 1);
    selectRow(row - 1);

    postup(row);
}

/**
 * @fn BasicTable::downSelected
 * @brief Moves the row of the selected table down one position.
 * @param postdown
 * Processing after a successful "down" operation in a table.
 * The argument will be the line number before the "down" operation.
 */
void BasicTable::downSelected(std::function<void (int)> postdown)
{
    if(this->rowCount() == 0
        || this->selectedItems().count() == 0) return;
    int row = this->row(this->selectedItems().at(0));

    if(row + 1 == this->rowCount()) return;
    swapTableRow(row, row + 1);
    selectRow(row + 1);

    postdown(row);
}

void BasicTable::copyToClipboard(QClipboard *clipboard)
{
    if(mimetypeclass.isEmpty()) return;

    TableMimeData *mime = new TableMimeData(mimetypeclass);
    mime->setTableData(tableSelectionToList());
    clipboard->setMimeData(mime);
}

const QList<QStringList> BasicTable::tableSelectionToList()
{
    QModelIndexList lists = this->selectionModel()->selectedRows();
    int colcount = this->columnCount();

    QList<QStringList> exprow;
    for(const auto &rowitem : lists){
        int rownum = rowitem.row();
        QStringList packed;
        for(int i = 0; i < colcount; i++){
            packed.append(
                rowitem.model()->index(rownum, i).data().toString());
        }

        exprow.append(packed);
    }

    return exprow;
}

/**
 * @fn BasicTable::pasteFromClipboard
 * @brief Paste from clipboard.
 * @param clipboard QApplication's clipboard.
 * @return True if pasted, false if not.
 */
bool BasicTable::pasteFromClipboard(QClipboard *clipboard)
{
    if(mimetypeclass.isEmpty()) return false;

    const QMimeData *mimedata = clipboard->mimeData();
    QString format = TableMimeData::getTableMimeType(mimetypeclass);
    if(!mimedata->hasFormat(format)) return false;

    const TableMimeData *mime
        = qobject_cast<const TableMimeData *>(clipboard->mimeData());

    QByteArray table = mime->data(format);

    const QList<QStringList> copiedrows
        = TableMimeData::getTableData(table);

    return insertItemFromList(copiedrows);
}

bool BasicTable::insertItemFromList(const QList<QStringList> &list)
{
    int row = this->currentRow();
    if(row < 0) return false;

    int columncount = this->columnCount();

    for(const auto &cols : list){
        insertRow(row);

        for(int i = 0; i < columncount; i++){
            this->setItem(row, i, new QTableWidgetItem(cols.at(i)));
        }
        row = this->currentRow();
    }

    return true;
}
