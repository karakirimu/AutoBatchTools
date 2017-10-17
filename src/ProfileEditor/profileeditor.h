#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#include "runtasksignalbinder.h"

#include <QMainWindow>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
//#include <pesharedfunction.h>
//#include <fileoperationsignalbinder.h>
#include <SettingDialog.h>
#include <editoperator.h>

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
    //file
    void newfileAction();
    void openAction();
    void saveAction();
    void overWriteSaveAction();
    void exportAction();

    //Edit
    void undoAction();
    void redoAction();
    void addAction();
//    void editAction();
//    void editAction(int);
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();

    void launchSettingAction();

//    void onWindowTitleChanged(QString newtitle, QString newtemp);
//    void itemChangedAction();
    void onTitleChanged(QString newload);
    void itemChangedAction(int index);
//    void editorTabChanged(int index);

    //tree acesss
    void setTreerowpos(int value, int from);

    void about();

private:

    void setLoadfile(const QString &value);

    //connect / disconnect signals data
//    void connectEdit();
//    void disconnectEdit();
    void initStatusBar();

    //set list infomation to ui
//    void setSettingList(QList<QStringList> *itemlist);

    //reset ui
    void resetUi();

    Ui::ProfileEditor *ui;
    SettingDialog *settingdialog;
    int treerowpos;
    QString loadfile;

//    PESharedFunction *sfunction;
//    FileOperationSignalBinder *binder;
    EditOperator *editop;
    RunTaskSignalBinder *rbinder;

//    void setSettingTempList(QList<QStringList> *list);
//    void setInfoList(QList<QStringList> *list, int firstpos);
//    void setNormalList(QList<QStringList> *list, int firstpos);
//    void setSearchList(QList<QStringList> *list, int firstpos);
//    void setScriptList(QList<QStringList> *list, int firstpos);

    //gather list information from ui
//    void createList(int type, QList<QStringList> *newlist);
//    void createTempList(QList<QStringList> *newlist);
//    void getInfoList(QList<QStringList> *newlist);
//    void getNormalList(QList<QStringList> *newlist);
//    void getSearchList(QList<QStringList> *newlist);
//    void getScriptList(QList<QStringList> *newlist);
//    void getOtherList(QList<QStringList> *newlist);
};

#endif // PROFILEEDITOR_H
