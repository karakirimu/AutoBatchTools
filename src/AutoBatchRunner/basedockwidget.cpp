#include "basedockwidget.h"

#include <QApplication>

BaseDockWidget::BaseDockWidget(QWidget *parent)
    : QDockWidget(parent), autohide(false)
{

    installEventFilter(this);
//    setMouseTracking(true);
//    setAttribute(Qt::WA_Hover);
    connect(this, &BaseDockWidget::topLevelChanged, this, &BaseDockWidget::showTitleBar);

}

BaseDockWidget::~BaseDockWidget()
{
    if(autohide) delete empty;
}

bool BaseDockWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(!this->isFloating() && autohide){

        if(event->type() == QEvent::Leave){
             setTitleBarWidget(empty);
        }

        if(event->type() == QEvent::ToolTip){
            if(widget()->mapFromGlobal(QCursor::pos()).y() <= Y_RANGE){
                setTitleBarWidget(def);
            }
        }

    }
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

        if(autohide){
            empty = new QWidget();
            def = titleBarWidget();
            setTitleBarWidget(empty);
        }else{
            setTitleBarWidget(def);
            delete empty;
        }
    }
}

void BaseDockWidget::showTitleBar()
{
    this->setTitleBarWidget(def);
}
