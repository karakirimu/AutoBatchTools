#ifndef FFPROBEEXTPIDWIDGET_H
#define FFPROBEEXTPIDWIDGET_H

#include <QWidget>

namespace Ui {
class FFprobeExtPidWidget;
}

class FFprobeExtPidWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FFprobeExtPidWidget(QWidget *parent = nullptr);
    ~FFprobeExtPidWidget();

private:
    Ui::FFprobeExtPidWidget *ui;
};

#endif // FFPROBEEXTPIDWIDGET_H
