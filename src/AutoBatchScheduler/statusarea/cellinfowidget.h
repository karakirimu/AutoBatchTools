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

signals:
    //send own objectname
    void consoleButtonClicked(QString);
    void pauseButtonClicked(QString);
    void stopButtonClicked(QString);

public slots:
    //init set
    void setProgressminmax(int start, int end);
    void setProfileName(QString name);

    //call when runprocess start or schedule started
    void started();
    void scheduled();

    //update information in runprocess
    void updateProcess(QString data, int type);
    void updateProgress(int num);



private slots:
    void onConsoleButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();

private:
    Ui::cellInfoWidget *ui;
};

#endif // CELLINFOWIDGET_H
