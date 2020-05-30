/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "executor_global.h"
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>
#include <../filesearchloader/filesearchloader.h>
#include <../plugins/plugininterface/extraplugininterface.h>
#include "../share/process/processxmlconstant.h"
#include "../share/process/functiontype.h"

#include <QObject>
#include <QProcess>
#include <QEventLoop>
#include <QTimer>
#include <QMutex>
#include <QStack>
#include <QPluginLoader>
#include <QFileInfo>

class EXECUTORSHARED_EXPORT Executor : public QObject
{   
    Q_OBJECT
public:
    explicit Executor(QObject *parent = nullptr);
    ~Executor();

    int getStartnum() const;
    void setStartnum(int start); /*preset func*/

    int getEndnum() const;
    void setEndnum(int end); /*preset func*/

    //run input
    void processWrite(QString code);
    void processKill();

    void setMutex(QMutex *mutex);

    bool getPaused() const;
    void setPaused(bool pause);

    QString macroConvert(QString part);

    void setExecList(QList<int> data);

    //status part
    bool getWorking() const;

    //xml part
    void setProcessFile(QString filepath); /*preset func*/

    int getLaunchedfrom() const;
    void setLaunchedfrom(int only = DEFAULT); /*preset func*/

    enum {INFO, NORMAL, SEARCH, PLUGINS, OTHER, TEMP, LOCAL, WARNING, ERROR, INPUT};
    enum {DEFAULT, SCHEDULER};

signals:
    //in runprocess
    //signals to all
    void processStarted(int);
    void processEnded(int);
    void processPaused();
    void processTimeout();
    void processStopped();

    //signals to ui (status)
    void processStateUpdate(int);
    void processErrorOccured(int);
    void processStateCount(int start, int end);

    //signals to textview (output)
    void processCheckError(QString);
    void processMessage(QString, int type);


public slots:
    //set slots
    /*inputmax is search only*/
    void addInputFiles(QStringList list, int inputmax = 1); /*preset func*/
    void setGlobalList(); /*preset func*/
    void setLocalList(); /*preset func*/

    //process slot
    bool runProcess(); /*main func*/
    void stopProcess();

private slots:
    void loadNormalStandardOutput();
    void sendPluginMessage(QString str){ emit processMessage(str, PLUGINS);}

private:
    //runprocess core
    bool loadInfo(QList<QStringList> *list);
    bool loadNormal(QList<QStringList> *list);
    bool loadSearch(QList<QStringList> *list);
    bool loadPlugins(QList<QStringList> *list);
    bool loadProject(QList<QStringList> *list);
    bool loadTemp(QList<QStringList> *list);

    bool Execute();

    //MacroString Converter
    QString replaceInputMacro(QString original);
    QString replaceGlobalMacro(QString original);
    QString replaceLocalMacro(QString original);
    QString replaceMacro(QString original, QHash<QString, QString> *list);

    //temp overwrite local macro
    void overwriteLocalMacro(QString key, QString value);

    //check execlist state
    void checkExecList(QList<int> *elist);

    //set processxmlbuilder's Infomation settings
    void setProcessSettings(bool *fileinput, int *loopcount);

    //common selector
//    int getReadType(QString type);

    //load resetting
    void resetdata();

    //sleep
    void sleep(int ms);

    //stop pause handler
    bool processStopHandleChecker();

    //for execution task variant
    typedef struct{
        //status part
        bool working = false;
        bool paused = false;
        //execute part
        QProcess *process;

    }WorkingParam;

    //execution setting task variant (normal)
    typedef struct{
        int launched = DEFAULT;
        int argumentscount;
        int othernestmax = 1;
        QString initFilename = "";
    }SettingParam;

    //execution setting task variant (test for profileeditor)
    typedef struct{
        int startnum = 0;
        int endnum = -1;
    }TestParam;

    WorkingParam *work;
    SettingParam *setting;
    TestParam *test;

    //setting part (execution order)
    QList<int> userexeclist;
    QList<int> *execlist;

    //inner
    bool neststop = false;
    bool noexeclist = false;

    //macro part
    QStringList fileList;
    QHash<QString, QString> globalHash;
    QHash<QString, QString> *localHash;

    //for file read secure
    QMutex *sMutex = nullptr;

    //xml part
    ProcessXmlBuilder *pbuilder;
    ProcessXmlListGenerator xgen;
    ProcessXmlConstant pxc;
    FunctionType ft;
    bool profileloaded = false;

    //plugin part
    ExtraPluginInterface *ext;

    //profileload part
    QStack<ProcessXmlBuilder *> builderstack;
    QStack<QHash<QString, QString> *> localstack;
    QStack<QList<int> *> execliststack;

    // local var
    const int LOCALVARINDEX = 1;

//    //xml attributes (temp)
//    const QString ALL_TYPE                   = "type";

//    // info
//    const QString I_NAME                     = "iname";
//    const QString I_VERSION                  = "ver";
//    const QString I_AUTHOR                   = "author";
//    const QString I_DESCRIPTION              = "desc";
//    const QString I_FILEINPUT                = "finput";
//    const QString I_FILEINPUT_SEARCHCHECK    = "sinput";
//    const QString I_FILESEARCH_NAME          = "fsname";
//    const QString I_RECURSIVE_LOOP           = "rloop";
//    const QString I_RECURSIVE_LOOPARGCOUNT   = "rlarg";
//    const QString I_RECURSIVE_LOOPCOUNT      = "reloop";

//    // search
//    const QString S_NAME                     = "sname";
//    const QString S_SEPARATOR                = "sep";
//    const QString S_VARIANT                  = "var";
//    const QString S_OUTPUTFILE               = "output";
//    const QString S_OUTPUTFILETYPE           = "fitype";

//    // profile

//    // XML tag "prname";
//    const QString PR_FILEPATH                = "prfile";

//    // exec(normal)
//    const QString E_TIMEOUT                  = "timeout";
//    const QString E_RUNDETACH                = "detach";
//    const QString E_CMD                      = "exc";
//    const QString E_CMDARGCOUNT              = "cmdc";

//    // plugin(extrafunc)
//    const QString PL_NAME                    = "plname";
//    const QString PL_FILEPATH                = "plfile";
//    const QString PL_CMD                     = "plc";
//    const QString PL_CMDARGCOUNT             = "pcmdc";

//    // temp
//    const QString TE_STACKEDWIDGET_POSITION  = "istack";

//    // local
//    const QString L_VARIANT                  = "lvar";
//    const QString L_VAR_COUNT                = "localc";

//    // value define (index 1)
//    const QString TYPE_INFO                  = "info";
//    const QString TYPE_EXEC                  = "normal";
//    const QString TYPE_SEARCH                = "search";
//    const QString TYPE_SCRIPT                = "script";
//    const QString TYPE_ANOTHER               = "other";

//    const QString TYPE_ALLINCLUDE            = "temp";
//    const QString TYPE_LOCAL                 = "local";

//    // attribute define (index 2)
//    // all
//    const QString ATTR_ONLY_SCHEDULER        = "only";
//    const QString ATTR_POSNUM                = "id";
//    const QString ATTR_RADIOBUTTONPOS        = "radio";
//    const QString ATTR_MAXCOUNT              = "max";
//    const QString ATTR_NONE                  = "";

//    // normal
//    const QString ATTR_TIMEOUTMS             = "dur";

//    // local
//    const QString ATTR_LOCALVALUE            = "lval";

};

#endif // EXECUTOR_H
