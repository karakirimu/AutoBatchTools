/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editorcacheconverter.h"

EditorCacheConverter::EditorCacheConverter()
{

}

EditorCacheConverter::~EditorCacheConverter()
{

}

/**
 * @fn EditorCacheConverter::convertToCacheList
 * @brief Convert XML to a cache for editor editing
 * @param source List from XML
 * @param dest Obtained data structure
 */
void EditorCacheConverter::convertToCacheList(
                                        const QList<QList<QStringList> *> *source,
                                        QList<EditorCache> *dest)
{
    dest->clear();
    for(QList<QStringList> *one : *source){
        EditorCache cache;
        convertToCache(&cache,one);
        dest->append(cache);
    }
}

/**
 * @fn EditorCacheConverter::convertToXml
 * @brief Converts a cache for editing into a list for writing to XML
 * @param source List from data structure
 * @param dest Obtained XML list
 */
void EditorCacheConverter::convertToXml(const QList<EditorCache> *source
                                        , QList<QList<QStringList> *> *dest)
{
    dest->clear();
    for(auto& cache : *source){
        QList<QStringList> *one = new QList<QStringList>();
        convertFromCache(&cache, one);
        dest->append(one);
    }
}

void EditorCacheConverter::convertToExportXml(const QList<EditorCache> *source
                                              , QList<QList<QStringList> *> *dest)
{
    dest->clear();
    for(auto& cache : *source){
        QList<QStringList> *one = new QList<QStringList>();
        convertFromCacheForExport(&cache, one);
        dest->append(one);
    }
}

/**
 * @fn EditorCacheConverter::convertFromCache
 * @brief Convert one element to XML format
 * @param from Data structure for gui operation
 * @param to Data structure for xml output
 */
void EditorCacheConverter::convertFromCache(const EditorCache *from,
                                            QList<QStringList> *to)
{
    switch (ft.getType(from->type)) {
    case ft.TYPE::ALLINCLUDE:
        to->append((QStringList() << pxc.TAG_TYPE
                                  << ft.getString(ft.TYPE::ALLINCLUDE)));
        to->append((QStringList() << pxc.TAG_FUNCTIONSELECT
                                  << QString::number(from->functionSelect)));
        fromExecuteCache(from, to);
        fromFileSearchCache(from, to);
        fromPluginCache(from, to);
        fromProfileLoadCache(from, to);
        break;
    case ft.TYPE::INFORMATION:
        fromInfomationCache(from, to);
        break;
    case ft.TYPE::LOCAL:
        fromLocalCache(from, to);
        break;
    case ft.TYPE::EXECUTE:
        fromExecuteCache(from, to);
        break;
    case ft.TYPE::FILESEARCH:
        fromFileSearchCache(from, to);
        break;
    case ft.TYPE::PLUGIN:
        fromPluginCache(from, to);
        break;
    case ft.TYPE::PROFILELOAD:
        fromProfileLoadCache(from, to);
        break;
    case ft.TYPE::INVALID:
        break;
    }
}

void EditorCacheConverter::fromInfomationCache(const EditorCache *from
                                               , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE << ft.getString(ft.TYPE::INFORMATION)));
    to->append((QStringList() << pxc.TAG_I_NAME << from->info.name));
    to->append((QStringList() << pxc.TAG_I_VERSION << from->info.version));
    to->append((QStringList() << pxc.TAG_I_AUTHOR << from->info.author));
    to->append((QStringList() << pxc.TAG_I_DESCRIPTION << from->info.description));
    to->append((QStringList() << pxc.TAG_I_FILEINPUT_BOOL
                              << VariantConverter::boolToString(from->info.fileInput)));
    to->append((QStringList() << pxc.TAG_I_FILEINPUT_SEARCH_BOOL
                              << VariantConverter::boolToString(from->info.fileInputSearch)));
    to->append((QStringList() << pxc.TAG_I_FILESEARCH_NAME
                              << from->info.fileSearchName
                              << pxc.ATTR_COMMAND_ID_INT
                              << QString::number(from->info.fileSearchIndex)));

    to->append((QStringList() << pxc.TAG_I_PROCESS_BOOL_HA1
                              << VariantConverter::boolToString(from->info.processAll)
                              << pxc.ATTR_I_PROCESSMAX_INT
                              << QString::number(from->info.processMaxCount)));

    to->append((QStringList() << pxc.TAG_I_ARG_IN_ONELOOP_INT
                              << QString::number(from->info.argumentsInOneLoop)));
    to->append((QStringList() << pxc.TAG_I_RECURSIVE_LOOPMAX_INT
                              << QString::number(from->info.recursiveLoopMax)));
    to->append((QStringList() << pxc.TAG_I_PROFILE_BASEPATH << from->info.basefilepath));
}

void EditorCacheConverter::fromLocalCache(const EditorCache *from
                                          , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE << ft.getString(ft.TYPE::LOCAL)));
    to->append((QStringList() << pxc.TAG_L_VARIANTCOUNT_INT
                              << QString::number(from->local.variantData.count())));

    for(VariantPair v : from->local.variantData){
        to->append(QStringList() << pxc.TAG_L_VARIANT_HA1 << v.variant
                                 << pxc.ATTR_L_VALUE << v.value);
    }
}

void EditorCacheConverter::fromExecuteCache(const EditorCache *from
                                            , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE
                              << ft.getString(ft.TYPE::EXECUTE)
                              << pxc.ATTR_ONLY_SCHEDULER_BOOL
                              << VariantConverter::boolToString(from->exec.schedulerOnly)));
    to->append((QStringList() << pxc.TAG_E_TIMEOUT_BOOL_HA1
                              << VariantConverter::boolToString(from->exec.timeoutEnabled)
                              << pxc.ATTR_TIMEOUT_INT
                              << QString::number(from->exec.timeout)));
    to->append((QStringList() << pxc.TAG_E_DETACH_BOOL
                              << VariantConverter::boolToString(from->exec.detach)));

    qsizetype commandCount = from->exec.command.count();
    to->append((QStringList() << pxc.TAG_E_COMMANDCOUNT_INT
                              << QString::number(commandCount)));

    for(int i = 0; i < commandCount; i++){
        to->append(QStringList() << pxc.TAG_E_CMD_HA1 << from->exec.command.at(i)
                                 << pxc.ATTR_COMMAND_ID_INT << QString::number(i));
    }
}

void EditorCacheConverter::fromFileSearchCache(const EditorCache *from
                                               , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE
                              << ft.getString(ft.TYPE::FILESEARCH)
                              << pxc.ATTR_ONLY_SCHEDULER_BOOL
                              << VariantConverter::boolToString(from->filesearch.schedulerOnly)));
    to->append((QStringList() << pxc.TAG_FS_NAME_HA1
                              << from->filesearch.name
                              << pxc.ATTR_COMMAND_ID_INT
                              << QString::number(from->filesearch.nameIndex)));
    to->append((QStringList() << pxc.TAG_FS_SEPARATOR << from->filesearch.separator));
    to->append((QStringList() << pxc.TAG_FS_VARIANT << from->filesearch.variant));
    to->append((QStringList() << pxc.TAG_FS_FILEPATH_HA1
                              << from->filesearch.filePath
                              << pxc.ATTR_FS_OUTPUTOPTION_INT
                              << QString::number(from->filesearch.outputOption)));
    to->append((QStringList() << pxc.TAG_FS_WRITEOPTION_INT
                              << QString::number(from->filesearch.writeOption)));
}

void EditorCacheConverter::fromPluginCache(const EditorCache *from
                                           , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE
                              << ft.getString(ft.TYPE::PLUGIN)
                              << pxc.ATTR_ONLY_SCHEDULER_BOOL
                              << VariantConverter::boolToString(from->plugin.schedulerOnly)));
    to->append((QStringList() << pxc.TAG_P_NAME << from->plugin.name));
    to->append((QStringList() << pxc.TAG_P_FILEPATH << from->plugin.filePath));

    int commandCount = static_cast<int>(from->plugin.command.count());
    to->append((QStringList() << pxc.TAG_P_COMMANDCOUNT_INT
                              << QString::number(commandCount)));

    for(int i = 0; i < commandCount; i++){
        to->append(QStringList() << pxc.TAG_P_CMD_HA1 << from->plugin.command.at(i)
                                 << pxc.ATTR_COMMAND_ID_INT << QString::number(i));
    }
}

void EditorCacheConverter::fromProfileLoadCache(const EditorCache *from
                                                , QList<QStringList> *to)
{
    to->append((QStringList() << pxc.TAG_TYPE
                              << ft.getString(ft.TYPE::PROFILELOAD)
                              << pxc.ATTR_ONLY_SCHEDULER_BOOL
                              << VariantConverter::boolToString(from->profileload.schedulerOnly)));
    to->append((QStringList() << pxc.TAG_PLOAD_FILEPATH << from->profileload.filePath));
}

/**
 * @fn EditorCacheConverter::convertToCache
 * @brief Convert one XML element into an edit data structure
 * @param to Data structure for gui operation
 * @param from Data structure for xml output
 */
void EditorCacheConverter::convertToCache(EditorCache *to, const QList<QStringList> *from)
{
    to->type = fetch(pxc.TAG_TYPE, from);

    switch (ft.getType(to->type)) {
    case ft.TYPE::ALLINCLUDE:
        to->functionSelect = fetch(pxc.TAG_FUNCTIONSELECT, from).toInt();
        toExecuteCache(to, from);
        toFileSearchCache(to, from);
        toPluginCache(to, from);
        toProfileLoadCache(to, from);
        break;
    case ft.TYPE::INFORMATION:
        toInfomationCache(to, from);
        break;
    case ft.TYPE::LOCAL:
        toLocalCache(to, from);
        break;
    case ft.TYPE::EXECUTE:
        to->functionSelect = static_cast<int>(TAB::EXECUTE);
        toExecuteCache(to, from);
        break;
    case ft.TYPE::FILESEARCH:
        to->functionSelect = static_cast<int>(TAB::FILESEARCH);
        toFileSearchCache(to, from);
        break;
    case ft.TYPE::PLUGIN:
        to->functionSelect = static_cast<int>(TAB::PLUGINS);
        toPluginCache(to, from);
        break;
    case ft.TYPE::PROFILELOAD:
        to->functionSelect = static_cast<int>(TAB::PROFILELOAD);
        toProfileLoadCache(to, from);
        break;
    case ft.TYPE::INVALID:
        break;
    }
}

void EditorCacheConverter::convertFromCacheForExport(const EditorCache *from
                                                     , QList<QStringList> *to)
{
    auto fromTabPosition = [&](){
        TAB tabpos = static_cast<TAB>(from->functionSelect);

        switch (tabpos) {
        case TAB::EXECUTE:
            fromExecuteCache(from, to);
            break;
        case TAB::FILESEARCH:
            fromFileSearchCache(from, to);
            break;
        case TAB::PLUGINS:
            fromPluginCache(from, to);
            break;
        case TAB::PROFILELOAD:
            fromProfileLoadCache(from, to);
            break;
        default:
            break;
        }
    };

    switch (ft.getType(from->type)) {
    case ft.TYPE::ALLINCLUDE:
        fromTabPosition();
        break;
    case ft.TYPE::INFORMATION:
        fromInfomationCache(from, to);
        break;
    case ft.TYPE::LOCAL:
        fromLocalCache(from, to);
        break;
    case ft.TYPE::EXECUTE:
        fromExecuteCache(from, to);
        break;
    case ft.TYPE::FILESEARCH:
        fromFileSearchCache(from, to);
        break;
    case ft.TYPE::PLUGIN:
        fromPluginCache(from, to);
        break;
    case ft.TYPE::PROFILELOAD:
        fromProfileLoadCache(from, to);
        break;
    case ft.TYPE::INVALID:
        break;
    }
}

void EditorCacheConverter::toInfomationCache(EditorCache *to
                                             , const QList<QStringList> *from)
{
    to->info.name = fetch(pxc.TAG_I_NAME, from);
    to->info.version = fetch(pxc.TAG_I_VERSION, from);
    to->info.author = fetch(pxc.TAG_I_AUTHOR, from);
    to->info.description = fetch(pxc.TAG_I_DESCRIPTION, from);

    to->info.fileInput = VariantConverter::stringToBool(fetch(pxc.TAG_I_FILEINPUT_BOOL, from));
    to->info.fileInputSearch = VariantConverter::stringToBool(fetch(pxc.TAG_I_FILEINPUT_SEARCH_BOOL, from));

    to->info.fileSearchName = fetch(pxc.TAG_I_FILESEARCH_NAME, from);
    to->info.fileSearchIndex = fetch(pxc.TAG_I_FILESEARCH_NAME, pxc.ATTR_COMMAND_ID_INT, from).toInt();

    to->info.processAll = VariantConverter::stringToBool(fetch(pxc.TAG_I_PROCESS_BOOL_HA1, from));
    to->info.processMaxCount = fetch(pxc.TAG_I_PROCESS_BOOL_HA1, pxc.ATTR_I_PROCESSMAX_INT, from).toInt();

    to->info.argumentsInOneLoop = fetch(pxc.TAG_I_ARG_IN_ONELOOP_INT, from).toInt();

    to->info.recursiveLoopMax = fetch(pxc.TAG_I_RECURSIVE_LOOPMAX_INT, from).toInt();

    to->info.basefilepath = fetch(pxc.TAG_I_PROFILE_BASEPATH, from);
}

void EditorCacheConverter::toLocalCache(EditorCache *to, const QList<QStringList> *from)
{
    int variableCount = fetch(pxc.TAG_L_VARIANTCOUNT_INT, from).toInt();
    int cmdfirst = fetchCommandFirstPos(pxc.TAG_L_VARIANTCOUNT_INT, from);

    for(int i = 0; i < variableCount; i++){
        VariantPair pair = {from->at(cmdfirst + i).at(1), from->at(cmdfirst + i).at(3)};
        to->local.variantData.append(pair);
    }
}

void EditorCacheConverter::toExecuteCache(EditorCache *to, const QList<QStringList> *from)
{
    int commandCount = fetch(pxc.TAG_E_COMMANDCOUNT_INT, from).toInt();

    to->exec.detach = VariantConverter::stringToBool(fetch(pxc.TAG_E_DETACH_BOOL, from));

    to->exec.timeoutEnabled = VariantConverter::stringToBool(fetch(pxc.TAG_E_TIMEOUT_BOOL_HA1, from));
    to->exec.timeout = fetch(pxc.TAG_E_TIMEOUT_BOOL_HA1,pxc.ATTR_TIMEOUT_INT, from).toInt();

    int cmdfirst = fetchCommandFirstPos(pxc.TAG_E_CMD_HA1, from);
    for(int i = 0; i < commandCount; i++){
        to->exec.command.append(from->at(cmdfirst + i).at(1));
    }

    to->exec.schedulerOnly = VariantConverter::stringToBool(fetch(pxc.TAG_TYPE,ft.getString(ft.TYPE::EXECUTE),pxc.ATTR_ONLY_SCHEDULER_BOOL, from));

}

void EditorCacheConverter::toFileSearchCache(EditorCache *to, const QList<QStringList> *from)
{
    to->filesearch.name = fetch(pxc.TAG_FS_NAME_HA1, from);
    to->filesearch.nameIndex = fetch(pxc.TAG_FS_NAME_HA1, pxc.ATTR_COMMAND_ID_INT, from).toInt();

    to->filesearch.separator = fetch(pxc.TAG_FS_SEPARATOR, from);

    to->filesearch.variant = fetch(pxc.TAG_FS_VARIANT, from);
    to->filesearch.filePath = fetch(pxc.TAG_FS_FILEPATH_HA1, from);

    // variable or outputfile
    to->filesearch.outputOption = fetch(pxc.TAG_FS_FILEPATH_HA1,pxc.ATTR_FS_OUTPUTOPTION_INT, from).toInt();

    // overwrite or append
    to->filesearch.writeOption = fetch(pxc.TAG_FS_WRITEOPTION_INT, from).toInt();

    to->filesearch.schedulerOnly = VariantConverter::stringToBool(fetch(pxc.TAG_TYPE,ft.getString(ft.TYPE::FILESEARCH),pxc.ATTR_ONLY_SCHEDULER_BOOL, from));
}

void EditorCacheConverter::toPluginCache(EditorCache *to, const QList<QStringList> *from)
{
    int commandCount = fetch(pxc.TAG_P_COMMANDCOUNT_INT, from).toInt();
    to->plugin.name = fetch(pxc.TAG_P_NAME, from);

    int ecmdfirst = fetchCommandFirstPos(pxc.TAG_P_CMD_HA1, from);
    for(int i = 0; i < commandCount; i++){
        to->plugin.command.append(from->at(ecmdfirst + i).at(1));
    }

    to->plugin.filePath = fetch(pxc.TAG_P_FILEPATH, from);
    to->plugin.schedulerOnly = VariantConverter::stringToBool(fetch(pxc.TAG_TYPE,ft.getString(ft.TYPE::PLUGIN),pxc.ATTR_ONLY_SCHEDULER_BOOL, from));
}

void EditorCacheConverter::toProfileLoadCache(EditorCache *to, const QList<QStringList> *from)
{
    to->profileload.filePath = fetch(pxc.TAG_PLOAD_FILEPATH, from);
    to->profileload.schedulerOnly = VariantConverter::stringToBool(fetch(pxc.TAG_TYPE,ft.getString(ft.TYPE::PROFILELOAD),pxc.ATTR_ONLY_SCHEDULER_BOOL, from));
}

/**
 * @fn EditorCacheConverter::fetch
 * @brief Get the value of an XML tag
 * @param tag Tag name
 * @param loadbase List for XML
 * @return The retrieved value
 */
QString EditorCacheConverter::fetch(QString tag,
                                    const QList<QStringList> *loadbase)
{
    return this->fetch(tag, "", loadbase, 0);
}

QString EditorCacheConverter::fetch(QString tag,
                                    QString attr,
                                    const QList<QStringList> *loadbase)
{
    return this->fetch(tag, attr, loadbase, 0);
}

QString EditorCacheConverter::fetch(QString tag,
                                    QString attr,
                                    const QList<QStringList> *loadbase,
                                    int firstpos)
{
    int count = static_cast<int>(loadbase->count());
    int i = firstpos;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == "") return loadbase->at(i).at(1);

            listnummax = static_cast<int>(loadbase->at(i).count());

            if(listnummax > 3 && attr == loadbase->at(i).at(2))
                return loadbase->at(i).at(3);

            if(listnummax > 5 && attr == loadbase->at(i).at(4))
                return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return "";
}

QString EditorCacheConverter::fetch(QString tag,
                                    QString value,
                                    QString attr,
                                    const QList<QStringList> *loadbase)
{
    int count = static_cast<int>(loadbase->count());
    int i = 0;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)
                && value == loadbase->at(i).at(1)){
            if(attr == "") return loadbase->at(i).at(1);

            listnummax = static_cast<int>(loadbase->at(i).count());

            if(listnummax > 3 && attr == loadbase->at(i).at(2))
                return loadbase->at(i).at(3);

            if(listnummax > 5 && attr == loadbase->at(i).at(4))
                return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return "";
}

/**
 * @fn XmlResolver::fetchCmdFirstPos
 * @brief Find the first position in the list that contains
 *        nformation about the table elements in the tab.
 * @param tag Tag name. (E_CMD or PL_CMD)
 * @param loadbase List of elements selected in ProcessFlowTable.
 * @return The first position in the list of table elements, or -1 if not found.
 */
int EditorCacheConverter::fetchCommandFirstPos(QString tag,
                                               const QList<QStringList> *loadbase)
{
    int count = static_cast<int>(loadbase->count());
    int i = 0;

    if(tag == pxc.TAG_E_CMD_HA1)  tag = pxc.TAG_E_COMMANDCOUNT_INT;
    if(tag == pxc.TAG_P_CMD_HA1) tag = pxc.TAG_P_COMMANDCOUNT_INT;

    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            return i + 1;
        }
        i++;
    }

    //cannot find
    return -1;
}
