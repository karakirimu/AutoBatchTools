#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QThread>
#include <runtasksignalbinder.h>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <../executor/executor.h>

#define RANGE 30

class ConsoleView : public QTextEdit
{
    Q_OBJECT
public:
    explicit ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();

    //ui settings

    void setRunTaskSignalBinder(RunTaskSignalBinder *rbinder);

private slots:
    void started(int runtype);
    void pause();
    void stop();
    void end(int runtype);

//    void ready();
    void updateText(QString data, int type);
    void updateExternalError(QString str);

    //context menu
    void onCustomContextMenu(const QPoint &point);
    void setPopupAction();

    //move position
    void moveCursorToEnd();

private:
    bool eventFilter(QObject *obj, QEvent *event);
//    QThread *workerthread;
//    Executor *exec;
    RunTaskSignalBinder *binder;

    //for send message to executor
    QString inputstr;

//    bool isReady = false;
    QMenu *contextMenu;
    QAction *m_selectall;
    QAction *m_copy;
    QAction *m_clear;
};

#endif // CONSOLEVIEW_H
