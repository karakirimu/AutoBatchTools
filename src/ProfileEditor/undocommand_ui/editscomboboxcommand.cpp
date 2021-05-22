/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editscomboboxcommand.h"

EditScomboBoxCommand::EditScomboBoxCommand(const int &targetindex
                                           , const QString &newstring
                                           , const int &newsearchindex
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newString = newstring;
    ptrCache = cache;
    newIndex = newsearchindex;
    select = cache->loadType(index);

    if (select == FunctionType::TYPE::INFORMATION){
        //info "fsname" position
        oldString = cache->at(index).info.fileSearchName;
        oldIndex = cache->at(index).info.fileSearchIndex;

    }else{
        // sname
        oldString = cache->at(index).filesearch.name;
        oldIndex = cache->at(index).filesearch.nameIndex;

    }
}

void EditScomboBoxCommand::undo()
{
    replace(oldString, oldIndex);
}

void EditScomboBoxCommand::redo()
{
    replace(newString, newIndex);
}

int EditScomboBoxCommand::id() const
{
    if(index > 1){
        // sname
        return UiCommandMap::FS_NAME_HA1;
    }else {
        //info "fsname" position
        return UiCommandMap::I_FILESEARCH_NAME;
    }
}

/**
 * @fn EditScomboBoxCommand::replace
 * @brief Swap the position and name corresponding to the combo box
 * @param str FileSearch name
 * @param i   The position corresponding to the name
 */
void EditScomboBoxCommand::replace(QString str, int i)
{
    EditorCache ec = ptrCache->at(index);
    QString message = "";

    if (select == FunctionType::TYPE::INFORMATION){
        //info "fsname" position
        ec.info.fileSearchName = str;
        ec.info.fileSearchIndex = i;

        message = QObject::tr("Change input filesearch condition to %1").arg(str);

    }else{
        // sname
        ec.filesearch.name = str;
        ec.filesearch.nameIndex = i;

        message = QObject::tr("Change filesearch condition to %1").arg(str);

    }
    ptrCache->replace(index, ec);
    setText(message + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}
