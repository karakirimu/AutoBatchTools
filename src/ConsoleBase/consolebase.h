#ifndef CONSOLEBASE_H
#define CONSOLEBASE_H

#include "consolebase_global.h"
#include <QWidget>
#include <QTextEdit>
#include <QThread>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <../executor/executor.h>

#define RANGE 30

class CONSOLEBASESHARED_EXPORT ConsoleBase : public QTextEdit
{
    Q_OBJECT
public:
    explicit ConsoleBase(QWidget *parent = nullptr);
    ~ConsoleBase();

protected slots:
    void started(int runtype);
    void pause();
    void stop();
    void end(int runtype);

    void updateText(QString data, int type);
    void updateExternalError(QString str);

    //context menu
    void onCustomContextMenu(const QPoint &point);
    void setPopupAction();

    //move position
    void moveCursorToEnd();

private:
    bool eventFilter(QObject *obj, QEvent *event);

    //for send message to executor
    QString inputstr;

    QMenu *contextMenu;
    QAction *m_selectall;
    QAction *m_copy;
    QAction *m_clear;
};

#endif // CONSOLEBASE_H
