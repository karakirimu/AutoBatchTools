#ifndef SEARCHDIALOGTABLE_H
#define SEARCHDIALOGTABLE_H

//#include <basetable.h>
#include <../basictable/basictable.h>
#include <QWidget>
#include <QClipboard>
#include <QHeaderView>
#include <QApplication>
#include <../fileinfodialog/fileinfodialog.h>

class SearchDialogTable : public BasicTable
{
    Q_OBJECT
public:
    explicit SearchDialogTable(QWidget *parent = nullptr);
    ~SearchDialogTable();

private slots:
    void copyAction();
    void propertyAction();

private:
    void setPopupActionTop();

    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_property;
};

#endif // SEARCHDIALOGTABLE_H
