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
    BASICTABLESHARED_EXPORT explicit BasicTable(QWidget *parent = Q_NULLPTR);
    BASICTABLESHARED_EXPORT ~BasicTable();

protected slots:
    BASICTABLESHARED_EXPORT virtual void editAction();
    BASICTABLESHARED_EXPORT virtual void addAction();
    BASICTABLESHARED_EXPORT virtual void deleteAction();
    BASICTABLESHARED_EXPORT virtual void copyAction();
    BASICTABLESHARED_EXPORT virtual void upAction();
    BASICTABLESHARED_EXPORT virtual void downAction();

    BASICTABLESHARED_EXPORT void horizontalHeaderClicked(int cols);

protected:
    BASICTABLESHARED_EXPORT virtual void setPopupActionTop();
    BASICTABLESHARED_EXPORT virtual void setPopupActionBottom();
    BASICTABLESHARED_EXPORT void setPopupActionDefault(QIcon copy, QIcon up, QIcon down);
    BASICTABLESHARED_EXPORT bool deleteCheckMessage();

    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;
};

#endif // LIBBASICTABLE_H
