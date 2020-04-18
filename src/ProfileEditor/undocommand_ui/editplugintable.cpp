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

EditPluginTable::EditPluginTable(const int &targetindex
                                     , const int &tableindex
                                     , QString newstr
                                     , const int operation
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_tableindex = tableindex;
    m_newstr = newstr;
    m_oldstr = "";
    m_operation = operation;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(m_targetindex));

    if(m_operation == ProcessXmlListGenerator::TABLE_EDIT
            || m_operation == ProcessXmlListGenerator::TABLE_DELETE
            || m_operation == ProcessXmlListGenerator::TABLE_CUT){
        m_oldstr = m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(1);
    }
}

/**
 * @fn EditPluginTable::EditPluginTable
 * @brief EditPluginTable constructor. It is used only for TABLE_ALLUPDATE operations.
 *
 * @param targetindex The number selected by ProcessFlowTable.
 * @param newstrlist  Updated new string list.
 * @param operation   Operation name for changing xml cache data. Enum is defined.
 * @param cache       Cache containing data to change.
 * @param parent      Parent object.
 */
EditPluginTable::EditPluginTable(const int &targetindex
                                     , QStringList newstrlist
                                     , const int operation
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstrlist = newstrlist;
    m_operation = operation;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(m_targetindex));
    int cmdcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();

    for(int i = 0; i < cmdcount; i++){
        m_oldstrlist.append(m_cache->at(m_targetindex)->at(i + SKIP).at(1));
    }
}

/**
 * @fn EditPluginTable::undo
 * @brief Undo operation.
 */
void EditPluginTable::undo()
{
    int rcount = -1;
    QStringList alist;
    QString sendcode;

    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Add plugin at %1").arg(QString::number(m_tableindex)) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL));

        break;

    case ProcessXmlListGenerator::TABLE_EDIT:
        pxlg.replaceElementList(m_tableindex, m_targetindex, m_oldstr, SKIP, m_cache);

        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(m_oldstr).arg(QString::number(m_tableindex)) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_PL_TABLEEDIT));
        break;

    case ProcessXmlListGenerator::TABLE_INSERT:
    case ProcessXmlListGenerator::TABLE_PASTE:
        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        sendcode = QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL);

        if(m_operation == ProcessXmlListGenerator::TABLE_PASTE){
            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex)).arg(m_newstr) + sendcode);
        }else{
            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex)).arg(m_newstr) + sendcode);
        }

        break;

    case ProcessXmlListGenerator::TABLE_DELETE:
    case ProcessXmlListGenerator::TABLE_CUT:
        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createPluginElement(m_oldstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        sendcode = QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_PL_TABLEINS);

        if(m_operation == ProcessXmlListGenerator::TABLE_CUT){
            setText(QObject::tr("Cut plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
        }else{
            setText(QObject::tr("Delete plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
        }

        break;

    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
        // delete all items
        for(int i = 0; i < rcount; i++){
            m_cache->at(m_targetindex)->removeAt(SKIP);
        }

        // add old items
        int tableindex = 0;
        for(QString item : m_oldstrlist){
            alist = ProcessXmlListGenerator::createPluginElement(item, tableindex);
            m_cache->at(m_targetindex)->append(alist);

            tableindex++;
        }

        // update counter
        QStringList ialist = m_cache->at(m_targetindex)->at(SKIP - 1);
        ialist.replace(1, QString::number(m_oldstrlist.count()));
        m_cache->at(m_targetindex)->replace(SKIP - 1, ialist);

        // set undo text
        QString strlistformat;
        for(QString str : m_oldstrlist){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UNDOREDO_PL_ALLUPDATE);
        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(m_targetindex)) + sendcode);
        break;
    }
}

/**
 * @fn EditPluginTable::redo
 * @brief Redo operation.
 */
void EditPluginTable::redo()
{
    QStringList alist;
    QString sendcode;
    int rcount = -1;

    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        alist = ProcessXmlListGenerator::createPluginElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Add plugin %1").arg(QString::number(m_tableindex)) \
                + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_PL_TABLEADD));

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        pxlg.replaceElementList(m_tableindex, m_targetindex, m_newstr, SKIP, m_cache);

        setText(QObject::tr("Edit plugin at %2 arg \'%1\'").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_PL_TABLEEDIT));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
    case ProcessXmlListGenerator::TABLE_PASTE:
        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createPluginElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        sendcode = QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_PL_TABLEINS);

        if(m_operation == ProcessXmlListGenerator::TABLE_PASTE){
            setText(QObject::tr("Paste plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex).arg(m_newstr)) + sendcode);
        }else{
            setText(QObject::tr("Insert plugin at %1 arg \'%2\'").arg(QString::number(m_tableindex).arg(m_newstr)) + sendcode);
        }

        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
    case ProcessXmlListGenerator::TABLE_CUT:
        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        sendcode = QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_PL_TABLEDEL);

        if(m_operation == ProcessXmlListGenerator::TABLE_CUT){
            setText(QObject::tr("Cut plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
        }else{
            setText(QObject::tr("Delete plugin at %1").arg(QString::number(m_tableindex)) + sendcode);
        }

        break;

    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
        rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();

        // delete all items
        for(int i = 0; i < rcount; i++){
            m_cache->at(m_targetindex)->removeAt(SKIP);
        }

        // add new items
        int tableindex = 0;
        for(QString item : m_newstrlist){
            alist = ProcessXmlListGenerator::createPluginElement(item, tableindex);
            m_cache->at(m_targetindex)->append(alist);

            tableindex++;
        }

        // update counter
        QStringList ialist = m_cache->at(m_targetindex)->at(SKIP - 1);
        ialist.replace(1, QString::number(m_newstrlist.count()));
        m_cache->at(m_targetindex)->replace(SKIP - 1, ialist);

        // update string
        QString strlistformat;
        for(QString str : m_newstrlist){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)").arg(strlistformat).arg(UNDOREDO_PL_ALLUPDATE);
        setText(QObject::tr("Update plugin settings at %1").arg(QString::number(m_targetindex)) + sendcode);
        break;
    }
}

int EditPluginTable::id() const
{
//    ProcessXmlListGenerator pxg;
//    return pxg.getId(PL_CMDARGCOUNT);
    ProcessXmlListGenerator pxg;

    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        return pxg.getId(PL_ADD_TABLE);

    case ProcessXmlListGenerator::TABLE_EDIT:
        return pxg.getId(PL_EDIT_TABLE);

    case ProcessXmlListGenerator::TABLE_INSERT:
        return pxg.getId(PL_INSERT_TABLE);

    case ProcessXmlListGenerator::TABLE_DELETE:
        return pxg.getId(PL_DELETE_TABLE);

    case ProcessXmlListGenerator::TABLE_ALLUPDATE:
        return pxg.getId(PL_ALLUPDATE_TABLE);
    }

    return pxg.getId(PL_CMDARGCOUNT);

}

bool EditPluginTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditPluginTable *com = static_cast<const EditPluginTable *>(other);
    if(m_operation == ProcessXmlListGenerator::TABLE_EDIT){
        m_newstr = com->m_newstr;
    }else{
        return false;
    }
    return true;
}

void EditPluginTable::updateIndex(int count)
{
    QStringList alist;
    for(int i = m_tableindex; i < count; i++){
        alist = m_cache->at(m_targetindex)->at(i + SKIP);
        alist.replace(3, QString::number(i));
        m_cache->at(m_targetindex)->replace(i + SKIP, alist);

    }
}

/**
 * @fn EditPluginTable::updateCounter
 * @brief Update the counter of commandtable recorded in cache.
 * @param ascend True to +1 the counter value, false to -1.
 */
void EditPluginTable::updateCounter(bool ascend)
{
    QStringList alist;
    //index size update
    alist = m_cache->at(m_targetindex)->at(SKIP - 1);
    int rcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    if(ascend){
        alist.replace(1, QString::number(rcount + 1));
    }else{
        alist.replace(1, QString::number(rcount - 1));
    }
    m_cache->at(m_targetindex)->replace(SKIP - 1, alist);
}
