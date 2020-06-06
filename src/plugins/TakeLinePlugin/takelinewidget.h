#ifndef TAKELINEWIDGET_H
#define TAKELINEWIDGET_H

#include <QWidget>

namespace Ui {
class TakeLineWidget;
}

class TakeLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TakeLineWidget(QWidget *parent = nullptr);
    ~TakeLineWidget();

private:
    Ui::TakeLineWidget *ui;
};

#endif // TAKELINEWIDGET_H
