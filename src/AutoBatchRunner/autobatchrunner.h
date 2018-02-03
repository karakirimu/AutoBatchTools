#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
//#include <systemtray.h>
#include <QProgressBar>
#include <QLabel>
#include <optiondialog.h>
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
    void on_actionOpen_triggered();
    void on_actionSettings_triggered();

    void on_runButton_clicked();

//    void on_editButton_clicked();

private:
    void initStatusBar();

    Ui::MainWindow *ui;
    OptionDialog *opdialog;
//    SystemTray *sysTray;
};

#endif // MAINWINDOW_H
