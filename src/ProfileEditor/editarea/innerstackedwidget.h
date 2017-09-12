#ifndef INNERSTACKEDWIDGET_H
#define INNERSTACKEDWIDGET_H

#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class InnerStackedWidget : public QStackedWidget
{
public:
    explicit InnerStackedWidget(QWidget *parent = nullptr);
    ~InnerStackedWidget();

    void moveStacked(int type);
public slots:
    void setInfoDataList(QList<QStringList> *list, int firstpos);
    void clearInfoDataListForm();
    void getInfoDataList(QStringList *list);
};

#endif // INNERSTACKEDWIDGET_H
