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

    if(tableOperation == UiCommandMap::E_EDIT_TABLE
            || tableOperation == UiCommandMap::E_DELETE_TABLE
            || tableOperation == UiCommandMap::E_CUT_TABLE){
        oldStr = cache->at(index).exec.command.at(tableIndex);
    }
}

void EditExecTable::undo()
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::E_ADD_TABLE:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add the execution argument to row %1 of No. %2").arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_DELETE_TABLE)));

        break;
    case UiCommandMap::E_EDIT_TABLE:

        list.replace(tableIndex, oldStr);
        setText(QObject::tr("Edit the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_EDIT_TABLE)));
        break;
    case UiCommandMap::E_INSERT_TABLE:
    case UiCommandMap::E_PASTE_TABLE:
        // Delete
        list.removeAt(tableIndex);

        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_DELETE_TABLE));
        if(tableOperation == UiCommandMap::E_PASTE_TABLE){
            setText(QObject::tr("Paste the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Insert the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }

        break;
    case UiCommandMap::E_DELETE_TABLE:
    case UiCommandMap::E_CUT_TABLE:
        // Insert
        list.insert(tableIndex, oldStr);

        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_INSERT_TABLE));
        if(tableOperation == UiCommandMap::E_CUT_TABLE){
            setText(QObject::tr("Cut the execution argument of row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Delete the execution argument of row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
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
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    QString sendcode;

    switch (tableOperation) {
    case UiCommandMap::E_ADD_TABLE:

        list.append(newStr);
        setText(QObject::tr("Add the execution argument to row %1 of No. %2").arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_ADD_TABLE)));

        break;
    case UiCommandMap::E_EDIT_TABLE:

        list.replace(tableIndex, newStr);
        // this is only all update
        setText(QObject::tr("Edit the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_EDIT_TABLE)));

        break;
    case UiCommandMap::E_INSERT_TABLE:
    case UiCommandMap::E_PASTE_TABLE:

        list.insert(tableIndex, newStr);
        sendcode = QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_INSERT_TABLE));

        if(tableOperation == UiCommandMap::E_PASTE_TABLE){
            setText(QObject::tr("Paste the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Insert the execution argument '%1' on row %2 of No. %3").arg(newStr).arg(tableIndex).arg(index) + sendcode);
        }

        break;
    case UiCommandMap::E_DELETE_TABLE:
    case UiCommandMap::E_CUT_TABLE:

        list.removeAt(tableIndex);
        sendcode = QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::E_DELETE_TABLE));

        if(tableOperation == UiCommandMap::E_CUT_TABLE){
            setText(QObject::tr("Cut the execution argument of row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
        }else{
            setText(QObject::tr("Delete the execution argument of row %1 of No. %2").arg(tableIndex).arg(index) + sendcode);
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
    return tableOperation;
}

bool EditExecTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditExecTable *com = static_cast<const EditExecTable *>(other);
    if(tableOperation == UiCommandMap::E_EDIT_TABLE){
        newStr = com->newStr;
    }else{
        return false;
    }
    return true;
}

int EditExecTable::operation() const
{
    return tableOperation;
}
