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

    void setTypeTest(int type);

    void setTypeAll(const QString type, const QPixmap *pixmap, \
                    const QString *colorCssText, const QString *frameCssText);
//    void setType(const QString type);
//    void setTypepixmap(const QPixmap *pixmap);
//    void setLabelcolor(const QString *cssText);
//    void setFramecolor(const QString *cssText);
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
    QPixmap blankpixmap;
//    FlowCellData celldata;

    Ui::FlowCellWidget *ui;

};

#endif // FLOWCELLWIDGET_H
