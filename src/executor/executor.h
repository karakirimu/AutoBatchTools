#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "executor_global.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"
#include "../processxmlbuilder/processxmlbuilder.h"
#include "../processxmllistgenerator/processxmllistgenerator.h"
#include "../variantconverter/variantconverter.h"
#include "../filesearchloader/filesearchloader.h"
#include "../plugins/ExtraPluginInterface/extraplugininterface.h"

#include <QObject>
#include <QProcess>
#include <QEventLoop>
#include <QTimer>
#include <QMutex>
#include <QStack>
#include <QPluginLoader>

class EXECUTORSHARED_EXPORT Executor : public QObject
{   
    Q_OBJECT
public:
    explicit Executor(QObject *parent = nullptr);
    ~Executor();

    //run part
    bool getDetached() const;
    void setDetached(bool detach); /*preset func*/

    int getStartnum() const;
    void setStartnum(int start); /*preset func*/

    int getEndnum() const;
    void setEndnum(int end); /*preset func*/

    //run input
    void processWrite(QString code);
    void processKill();

//    int getForcequittime() const;
//    void setForcequittime(int ms); /*preset func*/
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

    bool getSearchfileoverwrite() const;
    void setSearchfileoverwrite(bool overwrite); /*preset func*/

//    int getOthernestmax() const;
//    void setOthernestmax(int nest = 10);

    enum {INFO, NORMAL, SEARCH, SCRIPT, OTHER, TEMP, LOCAL, WARNING, ERROR, INPUT};
    enum {DEFAULT, SCHEDULER};
//    enum {MAINPROCESS, OTHERPROCESS};
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
//    void processInformation(QString);
    void processMessage(QString, int type);
//    void processUpdated(QString);


public slots:
    //set slots
    /*inputmax is search only*/
    void addInputFiles(QStringList list, int inputmax = 1); /*preset func*/
    void setGlobalList(); /*preset func*/
    void setLocalList(); /*preset func*/

    //process slot
    bool runProcess(); /*main func*/
//    bool runProcess2();
    void stopProcess();

private slots:
    void loadNormalStandardOutput();
//    void processEndLastStatus(int maxcount);
//    void extrafuncInternalMessage(QString str){ emit processMessage(str, SCRIPT);}

private:
    //runprocess core
    bool loadInfo(QList<QStringList> *list, int firstpos);
    bool loadNormal(QList<QStringList> *list, int firstpos);
    bool loadSearch(QList<QStringList> *list, int firstpos);
    bool loadPlugins(QList<QStringList> *list, int firstpos);
    bool loadOther(QList<QStringList> *list, int firstpos);
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
    int getReadType(QString type);

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
        bool detached = false;
        int launched = DEFAULT;
        bool searchoutputoverwrite = true;
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
//    int forcequittime = -1;

    //inner
    bool neststop = false;
    bool noexeclist = false;

    //macro part
//    QHash<QString, QString> fileHash;
    QStringList fileList;
    QHash<QString, QString> globalHash;
    QHash<QString, QString> *localHash;

    //for file read secure
    QMutex *sMutex = nullptr;

    //xml part
    ProcessXmlBuilder *pbuilder;
    ProcessXmlListGenerator xgen;
    bool profileloaded = false;

    //script part
    ExtraPluginInterface *ext;

    //otherprocess part
    QStack<ProcessXmlBuilder *> builderstack;
    QStack<QHash<QString, QString> *> localstack;
    QStack<QList<int> *> execliststack;
};

#endif // EXECUTOR_H
