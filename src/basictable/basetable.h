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
    BASICTABLESHARED_EXPORT explicit BaseTable(QWidget *parent = Q_NULLPTR);
    BASICTABLESHARED_EXPORT ~BaseTable();

protected slots:
    BASICTABLESHARED_EXPORT void onCustomContextMenu(const QPoint &point);
    BASICTABLESHARED_EXPORT void deleteTableRecursive();
    BASICTABLESHARED_EXPORT void copyTable(int index);

    BASICTABLESHARED_EXPORT QStringList selectFiles(QString basedir);
    BASICTABLESHARED_EXPORT QString selectFile(QString basedir);
    BASICTABLESHARED_EXPORT QString selectFolder(QString basedir);

protected:
    BASICTABLESHARED_EXPORT virtual void dropEvent(QDropEvent *event);
    BASICTABLESHARED_EXPORT void dragEnterEvent(QDragEnterEvent *event);
    BASICTABLESHARED_EXPORT QStringList droppedFromOutside(QDropEvent *event);
    BASICTABLESHARED_EXPORT void droppedFromInside(QDropEvent *event);
    BASICTABLESHARED_EXPORT void swapTableRow(int from, int dest);
    BASICTABLESHARED_EXPORT void insertTableRow(int from, int dest);
    BASICTABLESHARED_EXPORT virtual bool eventFilter(QObject *obj, QEvent *event);

    QMenu *contextMenu;

};

#endif // BASETABLE_H
