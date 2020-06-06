#ifndef SLEEPWIDGET_H
#define SLEEPWIDGET_H

#include <QWidget>

namespace Ui {
class SleepWidget;
}

class SleepWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SleepWidget(QWidget *parent = nullptr);
    ~SleepWidget();

private:
    Ui::SleepWidget *ui;
};

#endif // SLEEPWIDGET_H
