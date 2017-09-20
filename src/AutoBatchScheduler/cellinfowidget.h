#ifndef CELLINFOWIDGET_H
#define CELLINFOWIDGET_H

#include <QWidget>

namespace Ui {
class cellInfoWidget;
}

class cellInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cellInfoWidget(QWidget *parent = 0);
    ~cellInfoWidget();

private:
    Ui::cellInfoWidget *ui;
};

#endif // CELLINFOWIDGET_H
