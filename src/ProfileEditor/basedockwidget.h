#ifndef BASEDOCKWIDGET_H
#define BASEDOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QEvent>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QMutex>

//#define TIME_MS 1000
#define Y_RANGE 10

//class HoverTimer : public QThread
//{
//    Q_OBJECT
//public:
//    explicit HoverTimer(QObject *parent = nullptr);
//    ~HoverTimer();

//    void setEnter(bool value);
//    void setPoint(const QPoint &value);

//public slots:
//    void work();

//signals:
//    void launch();

//private:
////    QMutex mutex;
//    QPoint point;
//    bool enter;
//};

class BaseDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Autohide MEMBER autohide NOTIFY autohideOptionChanged)

public:
    explicit BaseDockWidget(QWidget *parent = nullptr);
    ~BaseDockWidget();

    bool isAutohide() const;
//    void mouseMoved();

public slots:
    void setAutohide(bool value);

signals:
    void autohideOptionChanged();

private slots:
    void showTitleBar();

private:
    bool eventFilter(QObject *watched, QEvent *event);

    //setting data
    bool autohide;

    //variant data
//    QThread *wait;
//    HoverTimer *hTimer;

    //widget data
    QWidget *def;
    QWidget *empty;
};

#endif // BASEDOCKWIDGET_H
