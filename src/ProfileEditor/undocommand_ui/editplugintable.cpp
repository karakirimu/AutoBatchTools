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

#include "editplugintable.h"

//EditPluginTable::EditPluginTable(const int &targetindex
//                                     , const int &tableindex
//                                     , QString newstr
//                                     , const int operation
//                                     , QList<QList<QStringList> *> *cache
//                                     , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    tableIndex = tableindex;
//    newStr = newstr;
//    oldStr = "";
//    tableOperation = operation;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(index));

//    if(tableOperation == ProcessXmlListGenerator::TABLE_EDIT
//            || tableOperation == ProcessXmlListGenerator::TABLE_DELETE
//            || tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//        oldStr = m_cache->at(index)->at(tableIndex + SKIP).at(1);
//    }
//}

///**
// * @fn EditPluginTable::EditPluginTable
// * @brief EditPluginTable constructor. It is used only for TABLE_ALLUPDATE operations.
// *
// * @param targetindex The number selected by ProcessFlowTable.
// * @param newstrlist  Updated new string list.
// * @param operation   Operation name for changing xml cache data. Enum is defined.
// * @param cache       Cache containing data to change.
// * @param parent      Parent object.
// */
//EditPluginTable::EditPluginTable(const int &targetindex
//                                     , QStringList newstrlist
//                                     , const int operation
//                                     , QList<QList<QStringList> *> *cache
//                                     , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newStrList = newstrlist;
//    tableOperation = operation;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(index));
//    int cmdcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();

//    for(int i = 0; i < cmdcount; i++){
//        oldStrList.append(m_cache->at(index)->at(i + SKIP).at(1));
//    }
//}

EditPluginTable::EditPluginTable(const int &targetindex
                                 , const int &tableindex
                                 , const QString newstr
                                 , const int operation
                                 , EditorCacheList *cache
                                 , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    tableIndex = tableindex;
    newStr = newstr;
    oldStr = "";
    tableOperation = operation;
    ptrCache = cache;

    if(tableOperation == UiCommandMap::PL_EDIT_TABLE
            || tableOperation == UiCommandMap::PL_DELETE_TABLE
            || tableOperation == UiCommandMap::PL_CUT_TABLE){
        oldStr = cache->at(index).plugin.command.at(tableIndex);
    }
}

EditPluginTable::EditPluginTable(const int &targetindex
                                 , const QStringList newstrlist
                                 , const int operation
                                 , EditorCacheList *cache
                                 , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStrList = newstrlist;
    tableOperation = operation;
    ptrCache = cache;

    oldStrList = cache->at(index).plugin.command;
}

/**
 * @fn EditPluginTable::undo
 * @brief Undo operation.
 */
void EditPluginTable::undo()
{
    // @deprecated
//    {
//    int rcount = -1;
//    QStringList alist;
//    QString sendcode;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        //delete
//        m_cache->at(index)->removeAt(m_tableindex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Add plugin at %1").arg(QString::number(m_tableindex)) \
//                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL));

//        break;

//    case ProcessXmlListGenerator::TABLE_EDIT:
//        pxlg.replaceElementList(m_tableindex, index, m_oldstr, SKIP, m_cache);

//        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(m_oldstr).arg(QString::number(m_tableindex)) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(m_tableindex).arg(UNDOREDO_PL_TABLEEDIT));
//        break;

//    case ProcessXmlListGenerator::TABLE_INSERT:
//    case ProcessXmlListGenerator::TABLE_PASTE:
//        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        m_cache->at(index)->removeAt(m_tableindex + SKIP);

//        updateCounter(false);

//        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

//        sendcode = QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
//            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex)).arg(m_newstr) + sendcode);
//        }else{
//            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex)).arg(m_newstr) + sendcode);
//        }

//        break;

//    case ProcessXmlListGenerator::TABLE_DELETE:
//    case ProcessXmlListGenerator::TABLE_CUT:
//        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        alist = ProcessXmlListGenerator::createPluginElement(m_oldstr, m_tableindex);
//        m_cache->at(index)->insert(m_tableindex + SKIP, alist);

//        updateCounter(true);

//        if(m_tableindex < rcount) updateIndex(rcount);

//        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(m_tableindex).arg(UNDOREDO_PL_TABLEINS);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//            setText(QObject::tr("Cut plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
//        }else{
//            setText(QObject::tr("Delete plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
//        }

//        break;

//    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
//        // delete all items
//        for(int i = 0; i < rcount; i++){
//            m_cache->at(index)->removeAt(SKIP);
//        }

//        // add old items
//        int tableindex = 0;
//        for(QString item : oldStrList){
//            alist = ProcessXmlListGenerator::createPluginElement(item, tableindex);
//            m_cache->at(index)->append(alist);

//            tableindex++;
//        }

//        // update counter
//        QStringList ialist = m_cache->at(index)->at(SKIP - 1);
//        ialist.replace(1, QString::number(oldStrList.count()));
//        m_cache->at(index)->replace(SKIP - 1, ialist);

//        // set undo text
//        QString strlistformat;
//        for(QString str : oldStrList){
//            strlistformat.append(str).append(",");
//        }

//        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UNDOREDO_PL_ALLUPDATE);
//        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(index)) + sendcode);
//        break;
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::PL_ADD_TABLE:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add plugin at %1").arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE)));
        break;

    case UiCommandMap::PL_EDIT_TABLE:

        list.replace(tableIndex, oldStr);
        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(oldStr).arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_EDIT_TABLE)));
        break;

    case UiCommandMap::PL_INSERT_TABLE:
    case UiCommandMap::PL_PASTE_TABLE:
        // Delete
        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE));
        if(tableOperation == UiCommandMap::PL_PASTE_TABLE){
            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
        }else{
            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
        }
        break;

    case UiCommandMap::PL_DELETE_TABLE:
    case UiCommandMap::PL_CUT_TABLE:
        // Insert
        list.insert(tableIndex, oldStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_INSERT_TABLE));

        if(tableOperation == UiCommandMap::PL_CUT_TABLE){
            setText(QObject::tr("Cut plugin at %1").arg(QString::number(tableIndex)) + sendcode);
        }else{
            setText(QObject::tr("Delete plugin at %1").arg(QString::number(tableIndex)) + sendcode);
        }
        break;

    case UiCommandMap::PL_ALLUPDATE_TABLE:

        list = oldStrList;

        // set undo text
        QString strlistformat;
        for(QString str : oldStrList){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UiCommandMap::Id(UiCommandMap::PL_ALLUPDATE_TABLE));
        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(index)) + sendcode);
        break;
    }

    ec.plugin.command = list;
    ptrCache->replace(index, ec);
}

/**
 * @fn EditPluginTable::redo
 * @brief Redo operation.
 */
void EditPluginTable::redo()
{
    // @deprecated
//    {
//    QStringList alist;
//    QString sendcode;
//    int rcount = -1;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        alist = ProcessXmlListGenerator::createPluginElement(m_newstr, m_tableindex);
//        m_cache->at(index)->insert(m_tableindex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Add plugin %1").arg(QString::number(m_tableindex)) \
//                + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_PL_TABLEADD));

//        break;
//    case ProcessXmlListGenerator::TABLE_EDIT:
//        pxlg.replaceElementList(m_tableindex, index, m_newstr, SKIP, m_cache);

//        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(m_newstr).arg(QString::number(m_tableindex)) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(m_tableindex).arg(UNDOREDO_PL_TABLEEDIT));
//        break;
//    case ProcessXmlListGenerator::TABLE_INSERT:
//    case ProcessXmlListGenerator::TABLE_PASTE:
//        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        alist = ProcessXmlListGenerator::createPluginElement(m_newstr, m_tableindex);
//        m_cache->at(index)->insert(m_tableindex + SKIP, alist);

//        updateCounter(true);

//        if(m_tableindex < rcount) updateIndex(rcount);

//        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(m_tableindex).arg(UNDOREDO_PL_TABLEINS);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
//            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex).arg(m_newstr)) + sendcode);
//        }else{
//            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex).arg(m_newstr)) + sendcode);
//        }

//        break;
//    case ProcessXmlListGenerator::TABLE_DELETE:
//    case ProcessXmlListGenerator::TABLE_CUT:
//        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        m_cache->at(index)->removeAt(m_tableindex + SKIP);

//        updateCounter(false);

//        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

//        sendcode = QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//            setText(QObject::tr("Cut plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
//        }else{
//            setText(QObject::tr("Delete plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
//        }

//        break;

//    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
//        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();

//        // delete all items
//        for(int i = 0; i < rcount; i++){
//            m_cache->at(index)->removeAt(SKIP);
//        }

//        // add new items
//        int tableindex = 0;
//        for(QString item : newStrList){
//            alist = ProcessXmlListGenerator::createPluginElement(item, tableindex);
//            m_cache->at(index)->append(alist);

//            tableindex++;
//        }

//        // update counter
//        QStringList ialist = m_cache->at(index)->at(SKIP - 1);
//        ialist.replace(1, QString::number(newStrList.count()));
//        m_cache->at(index)->replace(SKIP - 1, ialist);

//        // update string
//        QString strlistformat;
//        for(QString str : newStrList){
//            strlistformat.append(str).append(",");
//        }

//        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UNDOREDO_PL_ALLUPDATE);
//        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(index)) + sendcode);
//        break;
//    }
//    }


    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::PL_ADD_TABLE:

        list.append(newStr);
        setText(QObject::tr("Add plugin %1").arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::Id(UiCommandMap::PL_ADD_TABLE)));

        break;
    case UiCommandMap::PL_EDIT_TABLE:

        list.replace(tableIndex, newStr);
        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(newStr).arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_EDIT_TABLE)));
        break;
    case UiCommandMap::PL_INSERT_TABLE:
    case UiCommandMap::PL_PASTE_TABLE:

        list.insert(tableIndex, newStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_INSERT_TABLE));
        if(tableOperation == UiCommandMap::PL_PASTE_TABLE){
            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
        }else{
            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
        }

        break;
    case UiCommandMap::PL_DELETE_TABLE:
    case UiCommandMap::PL_CUT_TABLE:

        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE));
        if(tableOperation == UiCommandMap::PL_CUT_TABLE){
            setText(QObject::tr("Cut plugin at %1").arg(QString::number(tableIndex)) + sendcode);
        }else{
            setText(QObject::tr("Delete plugin at %1").arg(QString::number(tableIndex)) + sendcode);
        }

        break;

    case UiCommandMap::PL_ALLUPDATE_TABLE:
        list = newStrList;

        // update string
        QString strlistformat;
        for(QString str : newStrList){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UiCommandMap::Id(UiCommandMap::PL_ALLUPDATE_TABLE));
        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(index)) + sendcode);
        break;
    }

    ec.plugin.command = list;
    ptrCache->replace(index, ec);

}

int EditPluginTable::id() const
{
//    ProcessXmlListGenerator pxg;
//    return pxg.getId(PL_CMDARGCOUNT);
//    ProcessXmlListGenerator pxg;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        return pxg.getId(PL_ADD_TABLE);

//    case ProcessXmlListGenerator::TABLE_EDIT:
//        return pxg.getId(PL_EDIT_TABLE);

//    case ProcessXmlListGenerator::TABLE_INSERT:
//        return pxg.getId(PL_INSERT_TABLE);

//    case ProcessXmlListGenerator::TABLE_DELETE:
//        return pxg.getId(PL_DELETE_TABLE);

//    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
//        return pxg.getId(PL_ALLUPDATE_TABLE);
//    }

//    return pxg.getId(PL_CMDARGCOUNT);

    return tableOperation;

}

bool EditPluginTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditPluginTable *com = static_cast<const EditPluginTable *>(other);
    if(tableOperation == UiCommandMap::PL_EDIT_TABLE){
        newStr = com->newStr;
    }else{
        return false;
    }
    return true;
}

//void EditPluginTable::updateIndex(int count)
//{
//    QStringList alist;
//    for(int i = m_tableindex; i < count; i++){
//        alist = m_cache->at(index)->at(i + SKIP);
//        alist.replace(3, QString::number(i));
//        m_cache->at(index)->replace(i + SKIP, alist);

//    }
//}

///**
// * @fn EditPluginTable::updateCounter
// * @brief Update the counter of commandtable recorded in cache.
// * @param ascend True to +1 the counter value, false to -1.
// */
//void EditPluginTable::updateCounter(bool ascend)
//{
//    QStringList alist;
//    //index size update
//    alist = m_cache->at(index)->at(SKIP - 1);
//    int rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//    if(ascend){
//        alist.replace(1, QString::number(rcount + 1));
//    }else{
//        alist.replace(1, QString::number(rcount - 1));
//    }
//    m_cache->at(index)->replace(SKIP - 1, alist);
//}
