#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QAbstractButton>
#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void onAccept();
    void onButtonBoxClicked(QAbstractButton *button);
    void openDir();
//    void onReject();

private:
    Ui::SettingDialog *ui;
    void closeEvent(QCloseEvent *event);
    bool systemTraySelect();

    void setupItem();
    void setSettings();
    void loadSettings();
};

#endif // SETTINGDIALOG_H
