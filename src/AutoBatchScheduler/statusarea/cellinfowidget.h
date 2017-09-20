#ifndef CELLINFOWIDGET_H
#define CELLINFOWIDGET_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class cellInfoWidget;
}

class CellInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellInfoWidget(QWidget *parent = 0);
    ~CellInfoWidget();

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

    void setTimerEnd(QString date);

    void updateErrorProgress(int num);
    void updateErrorText(QString message);
private slots:
    void onConsoleButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();

private:
    Ui::cellInfoWidget *ui;
};

#endif // CELLINFOWIDGET_H
