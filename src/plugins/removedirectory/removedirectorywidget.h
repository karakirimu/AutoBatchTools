#ifndef REMOVEDIRECTRYWIDGET_H
#define REMOVEDIRECTRYWIDGET_H

#include <QWidget>

namespace Ui {
class RemoveDirectryWidget;
}

class RemoveDirectryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RemoveDirectryWidget(QWidget *parent = nullptr);
    ~RemoveDirectryWidget();

private:
    Ui::RemoveDirectryWidget *ui;
};

#endif // REMOVEDIRECTRYWIDGET_H
