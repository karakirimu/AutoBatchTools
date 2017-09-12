#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void onAccept();
    void onReject();

private:
    Ui::OptionDialog *ui;
    void closeEvent(QCloseEvent *event);
    bool systemTraySelect();

    void setupItem();
    void setSettings();
    void loadSettings();
};

#endif // OPTIONDIALOG_H
