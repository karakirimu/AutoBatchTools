#ifndef LIBBASICTABLE_H
#define LIBBASICTABLE_H

#include "basictable_global.h"
#include "basetable.h"
#include <QHeaderView>
#include <QMessageBox>

class BASICTABLESHARED_EXPORT BasicTable : public BaseTable
{
    Q_OBJECT
public:
    explicit BasicTable(QWidget *parent = Q_NULLPTR);
    ~BasicTable();

protected slots:
    virtual void editAction();
    virtual void addAction();
    virtual void deleteAction();
    virtual void copyAction();
    virtual void upAction();
    virtual void downAction();

    void horizontalHeaderClicked(int cols);

protected:
    virtual void setPopupActionTop();
    virtual void setPopupActionBottom();
    void setPopupActionDefault(QIcon copy, QIcon up, QIcon down);
    bool deleteCheckMessage();

    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;
};

#endif // LIBBASICTABLE_H
