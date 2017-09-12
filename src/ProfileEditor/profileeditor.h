#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#include "runtasksignalbinder.h"

#include <QMainWindow>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <pesharedfunction.h>
#include <fileoperationsignalbinder.h>
#include <SettingDialog.h>

namespace Ui {
class ProfileEditor;
}

class ProfileEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileEditor(QWidget *parent = 0);
    explicit ProfileEditor(QString loadfile, QWidget *parent = 0);
    ~ProfileEditor();

    void closeEvent(QCloseEvent *event);

signals:
    void rowPosChanged(QString);

private slots:
    void newfileAction();
    void openAction();
    void saveAction();
//  void addAction();
    void editAction();
    void editAction(int);
//    void copyAction();
    void overWriteSaveAction();
    void exportAction();

    void deleteAction();
    void upAction();
    void downAction();

    void launchSettingAction();

    void onWindowTitleChanged(QString newtitle, QString newtemp);
//    void itemChangedAction();
    void itemChangedAction(int index);
    void editorTabChanged(int index);

    void about();

private:
    Ui::ProfileEditor *ui;
    SettingDialog *settingdialog;
    int treerowpos;

    PESharedFunction *sfunction;
    FileOperationSignalBinder *binder;
    RunTaskSignalBinder *rbinder;
    //connect / disconnect signals data
    void connectEdit();
    void disconnectEdit();

    void initStatusBar();

    //set list infomation to ui
    void setSettingList(QList<QStringList> *itemlist);
//    void setSettingTempList(QList<QStringList> *list);
//    void setInfoList(QList<QStringList> *list, int firstpos);
//    void setNormalList(QList<QStringList> *list, int firstpos);
//    void setSearchList(QList<QStringList> *list, int firstpos);
//    void setScriptList(QList<QStringList> *list, int firstpos);

    //gather list information from ui
    void createList(int type, QList<QStringList> *newlist);
    void createTempList(QList<QStringList> *newlist);
    void getInfoList(QList<QStringList> *newlist, bool withtype);
    void getNormalList(QList<QStringList> *newlist, bool withtype);
    void getSearchList(QList<QStringList> *newlist, bool withtype);
    void getScriptList(QList<QStringList> *newlist, bool withtype);
    void getOtherList(QList<QStringList> *newlist, bool withtype);

    //reset ui
    void resetUi();
};

#endif // PROFILEEDITOR_H
