#include "basictable.h"


BasicTable::BasicTable(QWidget *){

    //set header sort settings
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(horizontalHeaderClicked(int)));
}

BasicTable::~BasicTable(){

}

void BasicTable::setPopupActionTop(){
    return;
}

void BasicTable::setPopupActionBottom(){
    return;
}

void BasicTable::setPopupActionDefault(QIcon copy, QIcon up, QIcon down)
{
    //set basic items
    m_copy = contextMenu->addAction(copy, tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_up = contextMenu->addAction(up, tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(down, tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_copy, SIGNAL(triggered()), this, SLOT(copyAction()));
    connect(m_up, SIGNAL(triggered()), this, SLOT(upAction()));
    connect(m_down, SIGNAL(triggered()), this, SLOT(downAction()));
}

bool BasicTable::deleteCheckMessage()
{
    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert"), tr("Do you want to delete selected files ?"), QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

void BasicTable::horizontalHeaderClicked(int)
{
    this->selectAll();
}

void BasicTable::editAction()
{
    return;
}

void BasicTable::addAction()
{
    return;
}

void BasicTable::deleteAction()
{
    return;
}

void BasicTable::copyAction()
{
    if(this->rowCount() == 0) return;
    copyTable(currentRow());
}

void BasicTable::upAction()
{
    if(this->rowCount() == 0) return;
    int row = this->row(this->selectedItems().at(0));

    if(row-1 == -1) return;
    swapTableRow(row,row-1);
}

void BasicTable::downAction()
{
    if(this->rowCount() == 0) return;
    int row = this->row(this->selectedItems().at(0));

    if(row+1 == this->rowCount()) return;
    swapTableRow(row,row+1);
}
