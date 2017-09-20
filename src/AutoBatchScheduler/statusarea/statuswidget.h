#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QDesktopWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();

public slots:

private:
    void closeEvent(QCloseEvent *event);

    Ui::StatusWidget *ui;
};

#endif // STATUSWIDGET_H
