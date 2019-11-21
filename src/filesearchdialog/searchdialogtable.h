#ifndef SEARCHDIALOGTABLE_H
#define SEARCHDIALOGTABLE_H

//#include <basetable.h>
#include <../basictable/basetable.h>
#include <QWidget>
#include <QClipboard>
#include <QHeaderView>
#include <QApplication>
#include <../fileinfodialog/fileinfodialog.h>

class SearchDialogTable : public BaseTable
{
    Q_OBJECT
public:
    explicit SearchDialogTable(QWidget *parent = nullptr);
    ~SearchDialogTable();

private slots:
    void copyAction();
    void propertyAction();

private:
    bool eventFilter(QObject *obj, QEvent *event);
    QAction *m_copy;
    QAction *m_property;
};

#endif // SEARCHDIALOGTABLE_H
