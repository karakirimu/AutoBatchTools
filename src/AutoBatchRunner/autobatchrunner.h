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
//    void on_actionConsole_triggered(bool checked);
//    void on_actionRun_triggered(bool checked);
//    void on_consoleDock_visibilityChanged(bool visible);
//    void on_runDock_visibilityChanged(bool visible);
    void on_actionSettings_triggered();

//    void showThisWindow();
//    void closedFromSystemTray();
    void on_runButton_clicked();

//    void on_editButton_clicked();

    void comboBoxItemChanged(int index);
    void on_actionOpen_triggered();

private:
//    void closeEvent(QCloseEvent *event);
//    bool systemTraySelect();

    void initStatusBar();

    Ui::MainWindow *ui;
    OptionDialog *opdialog;
//    SystemTray *sysTray;
};

#endif // MAINWINDOW_H
