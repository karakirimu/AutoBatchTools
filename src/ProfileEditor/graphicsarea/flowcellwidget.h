#ifndef FLOWCELLWIDGET_H
#define FLOWCELLWIDGET_H

#include <QWidget>

namespace Ui {
class FlowCellWidget;
}

class FlowCellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlowCellWidget(QWidget *parent = 0);
    ~FlowCellWidget();

private:
    Ui::FlowCellWidget *ui;
};

#endif // FLOWCELLWIDGET_H
