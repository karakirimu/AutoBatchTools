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

#include "editexectable.h"

//EditExecTable::EditExecTable(const int &targetindex
//                                       ,const int &tableindex
//                                       ,QString newstr
//                                       ,const int operation
//                                       ,QList<QList<QStringList> *> *cache
//                                       ,QUndoCommand *parent)
//    :QUndoCommand (parent)
//{
//    index = targetindex;
//    tableIndex = tableindex;
//    newStr = newstr;
//    oldStr = "";
//    tableOperation = operation;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(E_CMD, m_cache->at(index));

//    if(tableOperation == ProcessXmlListGenerator::TABLE_EDIT
//            || tableOperation == ProcessXmlListGenerator::TABLE_DELETE
//            || tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//        oldStr = m_cache->at(index)->at(tableIndex + SKIP).at(1);
//    }

//}

EditExecTable::EditExecTable(const int &targetindex
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

    if(tableOperation == ProcessXmlListGenerator::TABLE_EDIT
            || tableOperation == ProcessXmlListGenerator::TABLE_DELETE
            || tableOperation == ProcessXmlListGenerator::TABLE_CUT){
        oldStr = cache->at(index).exec.command.at(tableIndex);
    }
}

void EditExecTable::undo()
{
    // @deprecated
//    {
//    int rcount = -1;
//    QStringList alist;
//    QString sendcode;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        //delete
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Add exec at %1").arg(QString::number(tableIndex)) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL));
//        break;
//    case ProcessXmlListGenerator::TABLE_EDIT:

//        pxlg.replaceElementList(tableIndex, index, oldStr, SKIP, m_cache);

//        setText(QObject::tr("Edit exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEEDIT));
//        break;
//    case ProcessXmlListGenerator::TABLE_INSERT:
//    case ProcessXmlListGenerator::TABLE_PASTE:
//        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        if(tableIndex < (rcount-1)) updateIndex(rcount-1);

//        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
//            setText(QObject::tr("Paste exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
//        }else{
//            setText(QObject::tr("Insert exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
//        }

//        break;
//    case ProcessXmlListGenerator::TABLE_DELETE:
//    case ProcessXmlListGenerator::TABLE_CUT:
//        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        alist = ProcessXmlListGenerator::createExecElement(oldStr, tableIndex);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        if(tableIndex < rcount) updateIndex(rcount);

//        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEINS);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//            setText(QObject::tr("Cut exec at %1").arg(QString::number(tableIndex)) + sendcode);
//        }else{
//            setText(QObject::tr("Delete exec at %1").arg(QString::number(tableIndex)) + sendcode);
//        }

//        break;
//    default:
//        break;
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    QString sendcode;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add exec at %1").arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL));

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:

        list.replace(tableIndex, oldStr);
        setText(QObject::tr("Edit exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEEDIT));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
    case ProcessXmlListGenerator::TABLE_PASTE:
        // Delete
        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL);
        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
            setText(QObject::tr("Paste exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
        }else{
            setText(QObject::tr("Insert exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) + sendcode);
        }

        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
    case ProcessXmlListGenerator::TABLE_CUT:
        // Insert
        list.insert(tableIndex, oldStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEINS);
        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
            setText(QObject::tr("Cut exec at %1").arg(QString::number(tableIndex)) + sendcode);
        }else{
            setText(QObject::tr("Delete exec at %1").arg(QString::number(tableIndex)) + sendcode);
        }

        break;
    default:
        break;
    }

    ec.exec.command = list;
    ptrCache->replace(index, ec);

}

void EditExecTable::redo()
{
    // @deprecated
//    {
//    QStringList alist;
//    QString sendcode;
//    int rcount = -1;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        alist = ProcessXmlListGenerator::createExecElement(newStr, tableIndex);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Add exec at %1").arg(QString::number(tableIndex)) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEADD));
//        break;
//    case ProcessXmlListGenerator::TABLE_EDIT:
//        pxlg.replaceElementList(tableIndex, index, newStr, SKIP, m_cache);

//        // this is only all update
//        setText(QObject::tr("Edit exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEEDIT));
//        break;
//    case ProcessXmlListGenerator::TABLE_INSERT:
//    case ProcessXmlListGenerator::TABLE_PASTE:
//        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        alist = ProcessXmlListGenerator::createExecElement(newStr, tableIndex);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        if(tableIndex < rcount) updateIndex(rcount);

//        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEINS);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
//            setText(QObject::tr("Paste exec at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
//        }else{
//            setText(QObject::tr("Insert exec at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
//        }

//        break;
//    case ProcessXmlListGenerator::TABLE_DELETE:
//    case ProcessXmlListGenerator::TABLE_CUT:
//        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        if(tableIndex < (rcount-1)) updateIndex(rcount-1);

//        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL);

//        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
//            setText(QObject::tr("Cut exec at %1").arg(QString::number(tableIndex)) + sendcode);
//        }else{
//            setText(QObject::tr("Delete exec at %1").arg(QString::number(tableIndex)) + sendcode);
//        }

//        break;
//    default:
//        break;
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    QString sendcode;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:

        list.append(newStr);
        setText(QObject::tr("Add exec at %1").arg(QString::number(tableIndex)) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEADD));

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:

        list.replace(tableIndex, newStr);
        // this is only all update
        setText(QObject::tr("Edit exec at %1 arg \'%2\'").arg(QString::number(tableIndex)).arg(newStr) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEEDIT));

        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
    case ProcessXmlListGenerator::TABLE_PASTE:

        list.insert(tableIndex, newStr);
        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_E_TABLEINS);

        if(tableOperation == ProcessXmlListGenerator::TABLE_PASTE){
            setText(QObject::tr("Paste exec at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
        }else{
            setText(QObject::tr("Insert exec at %1 arg \'%2\'").arg(QString::number(tableIndex).arg(newStr)) + sendcode);
        }

        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
    case ProcessXmlListGenerator::TABLE_CUT:

        list.removeAt(tableIndex);
        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_E_TABLEDEL);

        if(tableOperation == ProcessXmlListGenerator::TABLE_CUT){
            setText(QObject::tr("Cut exec at %1").arg(QString::number(tableIndex)) + sendcode);
        }else{
            setText(QObject::tr("Delete exec at %1").arg(QString::number(tableIndex)) + sendcode);
        }

        break;
    default:
        break;
    }

    ec.exec.command = list;
    ptrCache->replace(index, ec);

}

int EditExecTable::id() const
{
    ProcessXmlListGenerator pxg;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        return pxg.getId(E_ADD_TABLE);

    case ProcessXmlListGenerator::TABLE_EDIT:
        return pxg.getId(E_EDIT_TABLE);

    case ProcessXmlListGenerator::TABLE_INSERT:
        return pxg.getId(E_INSERT_TABLE);

    case ProcessXmlListGenerator::TABLE_DELETE:
        return pxg.getId(E_DELETE_TABLE);

    }

    return pxg.getId(E_CMDARGCOUNT);
}

//bool EditExecTable::mergeWith(const QUndoCommand *other)
//{
//    if (other->id() != id()) return false;
//    const EditExecTable *com = static_cast<const EditExecTable *>(other);

//    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
//        m_newstr = com->m_newstr;
//    }else{
//        return false;
//    }

//    return true;
//}

int EditExecTable::operation() const
{
    return tableOperation;
}

//void EditExecTable::updateIndex(int count)
//{
//    QStringList alist;
//    for(int i = tableIndex; i < count; i++){
//        alist = m_cache->at(index)->at(i + SKIP);
//        alist.replace(3, QString::number(i));
//        m_cache->at(index)->replace(i + SKIP, alist);

//    }
//}

//void EditExecTable::updateCounter(bool ascend)
//{
//    QStringList alist;
//    //index size update
//    alist = m_cache->at(index)->at(SKIP - 1);
//    int rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(index))).toInt();
//    if(ascend){
//        alist.replace(1, QString::number(rcount + 1));
//    }else{
//        alist.replace(1, QString::number(rcount - 1));
//    }
//    m_cache->at(index)->replace(SKIP - 1, alist);
//}
