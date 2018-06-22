#ifndef BASETABLE_H
#define BASETABLE_H

#include "basictable_global.h"
#include <QTableWidget>
#include <QMenu>
#include <QWidget>
#include <QFileDialog>
#include <QMimeData>
#include <QMouseEvent>
#include <QDebug>

class BASICTABLESHARED_EXPORT BaseTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit BaseTable(QWidget *parent = Q_NULLPTR);
    ~BaseTable();

protected slots:
    void onCustomContextMenu(const QPoint &point);
    void deleteTableRecursive();
    void copyTable(int index);

    QStringList selectFiles(QString basedir);
    QString selectFile(QString basedir);
    QString selectFolder(QString basedir);

protected:
    virtual void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    QStringList droppedFromOutside(QDropEvent *event);
    void droppedFromInside(QDropEvent *event);
    void swapTableRow(int from, int dest);
    void insertTableRow(int from, int dest);
    virtual bool eventFilter(QObject *obj, QEvent *event);

    QMenu *contextMenu;

};

#endif // BASETABLE_H
