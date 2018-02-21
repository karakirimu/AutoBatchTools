#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
//#include <systemtray.h>
#include <QProgressBar>
#include <QLabel>
#include <optiondialog.h>
#include <multitask.h>
//#include <mainprocessloader.h>
//#include <profileeditor.h>

namespace Ui {
class MainWindow;
}

class AutoBatchRunner : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoBatchRunner(QWidget *parent = 0);
    ~AutoBatchRunner();

private slots:

    //received from task
    void taskStarted(QString objectname, int runfrom);
    void taskPaused(QString objectname);
    void taskStopped(QString objectname);
    void taskEnd(QString objectname, int runfrom);

    //send to task
    void on_actionOpen_triggered();
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void on_actionSettings_triggered();

    void on_editButton_clicked();

private:
    void initStatusBar();

    Ui::MainWindow *ui;
    OptionDialog *opdialog;
    MultiTask *mlTask;

    //this variable is temporary.
    //currently, this program can run only one task.
    QString key = "";
};

#endif // MAINWINDOW_H
