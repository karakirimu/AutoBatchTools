#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <startupxmlbuilder.h>
#include <../variantconverter/variantconverter.h>

#include <QDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartupDialog(QWidget *parent = 0);
    ~StartupDialog();

    void loadSettingList(int index, const QList<QStringList> *data);
protected slots:
    void onAccept();
    void onReject();
    void everyDaySelected(bool checked);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartupDialog *ui;
    StartupXmlBuilder *builder;
    bool editflag;
    int editindex;
    QString uniquecode;

    void createList(QList<QStringList> *newlist);

    QString daySelectToString();
    QString getRandomString(int length);
    QString timeToSeconds(QString data);
    QString secondsToTime(QString data);
    void StringToDaySelect(QString data);
};

#endif // STARTUPDIALOG_H
