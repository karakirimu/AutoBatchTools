/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editplugintable.h"

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
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::PL_ADD_TABLE:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add the plugin argument to row %1 of No. %2").arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE)));
        break;

    case UiCommandMap::PL_EDIT_TABLE:

        list.replace(tableIndex, oldStr);
        setText(QObject::tr("Edit the plugin argument '%1' on row %2 of No. %3").arg(oldStr).arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_EDIT_TABLE)));
        break;

    case UiCommandMap::PL_INSERT_TABLE:
    case UiCommandMap::PL_PASTE_TABLE:
        // Delete
        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE));
        if(tableOperation == UiCommandMap::PL_PASTE_TABLE){
            setText(QObject::tr("Paste the plugin argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Insert the plugin argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }
        break;

    case UiCommandMap::PL_DELETE_TABLE:
    case UiCommandMap::PL_CUT_TABLE:
        // Insert
        list.insert(tableIndex, oldStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_INSERT_TABLE));

        if(tableOperation == UiCommandMap::PL_CUT_TABLE){
            setText(QObject::tr("Cut the plugin argument for row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Delete the plugin argument for row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
        }
        break;

    case UiCommandMap::PL_ALLUPDATE_TABLE:

        list = oldStrList;

        // set undo text
        QString strlistformat;
        for(const QString& str : oldStrList){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)")
                       .arg(strlistformat, UiCommandMap::Id(UiCommandMap::PL_ALLUPDATE_TABLE));
        setText(QObject::tr("Update plugin settings at No. %1")
                    .arg(QString::number(index)) + sendcode);
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
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::PL_ADD_TABLE:

        list.append(newStr);
        setText(QObject::tr("Add the plugin argument to row %1 of No. %2").arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::Id(UiCommandMap::PL_ADD_TABLE)));

        break;
    case UiCommandMap::PL_EDIT_TABLE:

        list.replace(tableIndex, newStr);
        setText(QObject::tr("Edit the plugin argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_EDIT_TABLE)));
        break;
    case UiCommandMap::PL_INSERT_TABLE:
    case UiCommandMap::PL_PASTE_TABLE:

        list.insert(tableIndex, newStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_INSERT_TABLE));
        if(tableOperation == UiCommandMap::PL_PASTE_TABLE){
            setText(QObject::tr("Paste the plugin argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Insert the plugin argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }

        break;
    case UiCommandMap::PL_DELETE_TABLE:
    case UiCommandMap::PL_CUT_TABLE:

        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::PL_DELETE_TABLE));
        if(tableOperation == UiCommandMap::PL_CUT_TABLE){
            setText(QObject::tr("Cut the plugin argument for row %1 of No. %2").arg(tableIndex).arg(index)  + sendcode);
        }else{
            setText(QObject::tr("Delete the plugin argument for row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
        }

        break;

    case UiCommandMap::PL_ALLUPDATE_TABLE:
        list = newStrList;

        // update string
        QString strlistformat;
        for(const QString& str : newStrList){
            strlistformat.append(str).append(",");
        }

        sendcode = QString(" ^(%1%2)")
                       .arg(strlistformat, UiCommandMap::Id(UiCommandMap::PL_ALLUPDATE_TABLE));
        setText(QObject::tr("Update plugin settings at No. %1").arg(QString::number(index)) + sendcode);
        break;
    }

    ec.plugin.command = list;
    ptrCache->replace(index, ec);

}

int EditPluginTable::id() const
{
    return tableOperation;
}

bool EditPluginTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditPluginTable *com = static_cast<const EditPluginTable *>(other);
    if(tableOperation == UiCommandMap::PL_EDIT_TABLE
        && index == com->index
        && tableIndex == com->tableIndex){
        newStr = com->newStr;
    }else{
        return false;
    }
    return true;
}
