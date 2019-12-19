#ifndef BASEDOCKWIDGET_H
#define BASEDOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QEvent>
#include <QDebug>

#define Y_RANGE 10

class BaseDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Autohide MEMBER autohide NOTIFY autohideOptionChanged)

public:
    explicit BaseDockWidget(QWidget *parent = nullptr);
    ~BaseDockWidget();

    bool isAutohide() const;

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

    //widget data
    QWidget *def;
    QWidget *empty;
};

#endif // BASEDOCKWIDGET_H
