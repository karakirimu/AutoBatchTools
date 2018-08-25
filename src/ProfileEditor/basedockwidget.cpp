#include "basedockwidget.h"

BaseDockWidget::BaseDockWidget(QWidget *parent)
    : QDockWidget(parent), autohide(false)
{

    installEventFilter(this);
    def = titleBarWidget();
    empty = new QWidget;
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    connect(this, &BaseDockWidget::topLevelChanged, this, &BaseDockWidget::showTitleBar);
}

BaseDockWidget::~BaseDockWidget()
{
    delete empty;
}

bool BaseDockWidget::eventFilter(QObject *watched, QEvent *event)
{
//    if(!this->isFloating() && autohide){
//        qDebug() << "Dock" << event->type();
//        if(event->type() == QEvent::Enter){
//            wait = new QThread();
//            hTimer = new HoverTimer();
//            connect(wait, &QThread::started, hTimer, &HoverTimer::work);
//            connect(hTimer, &HoverTimer::launch, this, &BaseDockWidget::showTitleBar);

//            wait->moveToThread(hTimer);

////            hTimer->setEnter(true);
//        }

//        if(event->type() == QEvent::Leave){
////            hTimer->setEnter(false);
//            this->setTitleBarWidget(empty);

////            if(hTimer->isRunning()){
////                hTimer->quit();
////                hTimer->wait();

////                delete hTimer;
////            }

//            if(wait->isRunning() || hTimer->isRunning()){
//                hTimer->quit();
//                hTimer->wait();
//                wait->quit();
//                wait->wait();

//                delete hTimer;
//                delete wait;
//            }
//        }

//        if(event->type() == QEvent::HoverMove){
//            if(widget()->mapFromGlobal(QCursor::pos()).y() <= Y_RANGE){
//    //            hTimer->setInarea(true);
//                hTimer->setPoint(QCursor::pos());
//                mouseMoved();
//            }/*else{
//                hTimer->setInarea(false);
//            }*/
//        }

        if(!isFloating() && autohide && event->type() == QEvent::Leave){
             setTitleBarWidget(empty);
        }

        if(autohide && event->type() == QEvent::ToolTip){
            if(widget()->mapFromGlobal(QCursor::pos()).y() <= Y_RANGE){
    //            hTimer->setInarea(true);
//                hTimer->setPoint(QCursor::pos());
//                mouseMoved();
                setTitleBarWidget(def);
            }/*else{
                hTimer->setInarea(false);
            }*/
        }

//    }

//    qDebug() << "float" << this->isFloating();

    return QObject::eventFilter(watched, event);
}

bool BaseDockWidget::isAutohide() const
{
    return autohide;
}

void BaseDockWidget::setAutohide(bool value)
{
    if(autohide == value){
        return;
    }else{
        autohide = value;

        if(!this->isFloating() && autohide){
//            empty = new QWidget();
//            def = titleBarWidget();
            setTitleBarWidget(empty);
        }else{
            setTitleBarWidget(def);
//            delete empty;
        }
    }
}

//void BaseDockWidget::mouseMoved()
//{
//    wait->start();
//}

void BaseDockWidget::showTitleBar()
{
    this->setTitleBarWidget(def);
}

//HoverTimer::HoverTimer(QObject *parent) : QThread(parent)
//{

//}

//HoverTimer::~HoverTimer()
//{
//    qDebug() << "thread deleted";
//}

//void HoverTimer::work()
//{
////    mutex.lock();
//    QTime t;
//    t.start();

//    while (/*enter && */t.elapsed() < TIME_MS){
//        QApplication::processEvents();
//    }
////    mutex.unlock();

//    if(/*enter &&*/ point == QCursor::pos() && t.elapsed() >= TIME_MS){
//        qDebug() << "launched";
//        emit launch();
//    }
//}

//void HoverTimer::setPoint(const QPoint &value)
//{
//    point = value;
//}

//void HoverTimer::setEnter(bool value)
//{
//    enter = value;
//}
