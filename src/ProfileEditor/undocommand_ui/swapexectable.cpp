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

#include "swapexectable.h"

//SwapExecTable::SwapExecTable(const int &targetindex
//                             , const int &tablebefore
//                             , const int &tableafter
//                             , QList<QList<QStringList> *> *cache
//                             , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    indexBefore = tablebefore;
//    indexAfter = tableafter;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(E_CMD, m_cache->at(index));

//}

SwapExecTable::SwapExecTable(const int &targetindex
                             , const int &tablebefore
                             , const int &tableafter
                             , EditorCacheList *cache
                             , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    indexBefore = tablebefore;
    indexAfter = tableafter;
    ptrCache = cache;

    //SKIP = pxlg.fetchCmdFirstPos(E_CMD, m_cache->at(m_targetindex));

}

void SwapExecTable::undo()
{
    // @deprecated
//    {
//    QStringList temp;
//    temp = m_cache->at(index)->at(m_indexbefore + SKIP);

//    //swap
//    m_cache->at(index)->replace(m_indexbefore + SKIP
//                                        , m_cache->at(index)->at(m_indexafter + SKIP));
//    m_cache->at(index)->replace(m_indexafter + SKIP, temp);

//    int condition = m_indexafter - m_indexbefore;
//    switch (condition) {
//    case 1:
//        //UP
//        setText(QObject::tr("Up exec arg '%1'").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    case -1:
//        //DOWN
//        setText(QObject::tr("Down exec arg '%1'").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    default:
//        //SWAP
//        setText(QObject::tr("Swap exec arg '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    }
//    }

    replaceValue(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);

}

void SwapExecTable::redo()
{
    // @deprecated
//    {
//    QStringList temp;
//    temp = m_cache->at(index)->at(m_indexafter + SKIP);

//    //swap
//    m_cache->at(index)->replace(m_indexafter + SKIP
//                                        , m_cache->at(index)->at(m_indexbefore + SKIP));
//    m_cache->at(index)->replace(m_indexbefore + SKIP, temp);

//    int condition = m_indexafter - m_indexbefore;
//    switch (condition) {
//    case 1:
//        //UP
//        setText(QObject::tr("Up exec arg '%1'").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    case -1:
//        //DOWN
//        setText(QObject::tr("Down exec arg '%1'").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    default:
//        //SWAP
//        setText(QObject::tr("Swap exec arg '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
//                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_E_TABLESWAP));
//        break;
//    }
//    }

    replaceValue(indexBefore, indexAfter);
    setTextByCondition(indexBefore, indexAfter);

}

int SwapExecTable::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_SWAP_TABLE);
}

void SwapExecTable::replaceValue(int source, int dest)
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    list.swapItemsAt(source, dest);
    ec.exec.command = list;
    ptrCache->replace(index, ec);
}

void SwapExecTable::setTextByCondition(int source, int dest)
{
    int condition = dest - source;
    QString arg = ptrCache->at(index).exec.command.at(dest);

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up exec arg '%1'").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_E_TABLESWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down exec arg '%1'").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_E_TABLESWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap exec arg '%1' at %2 to %3").arg(arg).arg(source).arg(dest) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_E_TABLESWAP));
        break;
    }
}
