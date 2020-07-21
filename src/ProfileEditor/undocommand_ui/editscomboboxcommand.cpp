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

        message = QObject::tr("Input filesearch profile to");

    }else{
        // sname
        ec.filesearch.name = str;
        ec.filesearch.nameIndex = i;

        message = QObject::tr("Filesearch profile to");

    }
    ptrCache->replace(index, ec);
    setText(message + str + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}
