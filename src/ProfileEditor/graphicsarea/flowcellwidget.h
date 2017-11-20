#ifndef FLOWCELLWIDGET_H
#define FLOWCELLWIDGET_H

//#include "flowcelldata.h"

#include <QMouseEvent>
#include <QWidget>
#include <QSettings>

namespace Ui {
class FlowCellWidget;
}

class FlowCellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlowCellWidget(QWidget *parent = 0);
    ~FlowCellWidget();

    void setType(QString type);
    void setTypepixmap(QPixmap pixmap);
    void setTypecolor(QString cssText);
    void setFramecolor(QString cssText);
    void setContent(QString htmlText);
//    void setCellData(FlowCellData data);

    void hideArrow();
    void showArrow();

//    FlowCellData getCelldata() const;

public slots:
    void selectedItem();
    void unSelectedItem();

private:
//    void mousePressEvent(QMouseEvent *event);
    QString framecolor;
    QPixmap arrowpixmap;
//    FlowCellData celldata;

    Ui::FlowCellWidget *ui;
};

#endif // FLOWCELLWIDGET_H
