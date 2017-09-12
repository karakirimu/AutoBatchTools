#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "executor_global.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"
#include "../processxmlbuilder/processxmlbuilder.h"
#include "../variantconverter/variantconverter.h"
#include "../filesearchloader/filesearchloader.h"
#include "../plugins/RunnerExtraPluginInterface/runnerextraplugininterface.h"

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
    EXECUTORSHARED_EXPORT explicit Executor(QObject *parent = nullptr);
    EXECUTORSHARED_EXPORT ~Executor();

    //run part
    EXECUTORSHARED_EXPORT bool getDetached() const;
    EXECUTORSHARED_EXPORT void setDetached(bool detach); /*preset func*/

    EXECUTORSHARED_EXPORT int getStartnum() const;
    EXECUTORSHARED_EXPORT void setStartnum(int start); /*preset func*/

    EXECUTORSHARED_EXPORT int getEndnum() const;
    EXECUTORSHARED_EXPORT void setEndnum(int end); /*preset func*/

    EXECUTORSHARED_EXPORT int getForcequittime() const;
    EXECUTORSHARED_EXPORT void setForcequittime(int ms); /*preset func*/

    EXECUTORSHARED_EXPORT bool getPaused() const;
    EXECUTORSHARED_EXPORT void setPaused(bool pause);

    EXECUTORSHARED_EXPORT QString macroConvert(QString part);

    EXECUTORSHARED_EXPORT void setExecList(QList<int> data);

    //status part
    EXECUTORSHARED_EXPORT bool getWorking() const;

    //xml part
    EXECUTORSHARED_EXPORT void setProcessFile(QString filepath); /*preset func*/

    EXECUTORSHARED_EXPORT int getLaunchedfrom() const;
    EXECUTORSHARED_EXPORT void setLaunchedfrom(int only = DEFAULT); /*preset func*/

    EXECUTORSHARED_EXPORT bool getSearchfileoverwrite() const;
    EXECUTORSHARED_EXPORT void setSearchfileoverwrite(bool overwrite); /*preset func*/

    EXECUTORSHARED_EXPORT int getOthernestmax() const;
    EXECUTORSHARED_EXPORT void setOthernestmax(int nest = 10);

    enum {INFO, NORMAL, SEARCH, SCRIPT, OTHER, TEMP, LOCAL, ERROR};
    enum {DEFAULT, SCHEDULER};
    enum {MAINPROCESS, OTHERPROCESS};
signals:
    //in runprocess
    //signals to all
    EXECUTORSHARED_EXPORT void processStarted(int);
    EXECUTORSHARED_EXPORT void processEnded(int);
    EXECUTORSHARED_EXPORT void processPaused();
    EXECUTORSHARED_EXPORT void processTimeout();
    EXECUTORSHARED_EXPORT void processStopped();

    //signals to ui (status)
    EXECUTORSHARED_EXPORT void processStateUpdate(int);
    EXECUTORSHARED_EXPORT void processErrorOccured(int);
    EXECUTORSHARED_EXPORT void processStateCount(int start, int end);

    //signals to textview (output)
    EXECUTORSHARED_EXPORT void processCheckError(QString);
//    EXECUTORSHARED_EXPORT void processInformation(QString);
    EXECUTORSHARED_EXPORT void processMessage(QString, int type);
//    EXECUTORSHARED_EXPORT void processUpdated(QString);


public slots:
    //set slots
    /*inputmax is search only*/
    EXECUTORSHARED_EXPORT void addInputFiles(QStringList list, int inputmax = 1); /*preset func*/
    EXECUTORSHARED_EXPORT void setGlobalList(); /*preset func*/
    EXECUTORSHARED_EXPORT void setLocalList(); /*preset func*/

    //process slot
    EXECUTORSHARED_EXPORT bool runProcess(); /*main func*/
    EXECUTORSHARED_EXPORT void stopProcess();

private slots:
    void loadNormalStandardOutput();
    void processEndLastStatus();
//    void extrafuncInternalMessage(QString str){ emit processMessage(str, SCRIPT);}

private:
    //runprocess core
    bool loadInfo(QList<QStringList> *list, int firstpos);
    bool loadNormal(QList<QStringList> *list, int firstpos);
    bool loadSearch(QList<QStringList> *list, int firstpos);
    bool loadScript(QList<QStringList> *list, int firstpos);
    bool loadOther(QList<QStringList> *list, int firstpos);
    bool loadTemp(QList<QStringList> *list);

    //MacroString Converter
    QString replaceInputMacro(QString original);
    QString replaceGlobalMacro(QString original);
    QString replaceLocalMacro(QString original);
    QString replaceMacro(QString original, QHash<QString, QString> *list);

    //temp overwrite local macro
    void overwriteLocalMacro(QString key, QString value);

    //check execlist state
    void checkExecList();

    //common selector
    int getReadType(QString type);

    //load resetting
    void resetdata();

    //sleep
    void sleep(int ms);

    //status part
    bool working = false;
    bool paused = false;

    //setting part
    bool detached = false;
    int startnum = 0;
    int endnum = -1;
    QList<int> execlist;
    int forcequittime = -1;
    int launchedfrom = DEFAULT;
    bool autoaddexec = false;

    bool searchfileoverwrite = true;

    int othernestmax;
    bool neststop = false;

    //macro part
    QHash<QString, QString> fileHash;
    QHash<QString, QString> globalHash;
    QHash<QString, QString> *localHash;

    //xml part
    ProcessXmlBuilder *pbuilder;
    bool processfileloaded = false;

    //execute part
    QProcess *process;

    //script part
    RunnerExtraPluginInterface *ext;

    //otherprocess part
    QStack<ProcessXmlBuilder *> builderstack;
    QStack<QHash<QString, QString> *> localstack;
};

#endif // EXECUTOR_H
