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

#include "swaplocalvarianttree.h"

//SwapLocalVariantTree::SwapLocalVariantTree(const int &localindex
//                                         , const int &before
//                                         , const int &after
//                                         , QList<QList<QStringList> *> *cache
//                                         , QUndoCommand *parent)
//    :QUndoCommand (parent)
//{
//    index = localindex;
//    indexBefore = before;
//    indexAfter = after;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(index));
//}

SwapLocalVariantTree::SwapLocalVariantTree(const int &localindex
                                           , const int &before
                                           , const int &after
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand (parent)
{
    index = localindex;
    indexBefore = before;
    indexAfter = after;
    ptrCache = cache;
}

void SwapLocalVariantTree::undo()
{
//    QStringList temp;
//    temp = m_cache->at(index)->at(indexBefore + SKIP);

//    //swap
//    m_cache->at(index)->replace(indexBefore + SKIP
//                                        , m_cache->at(index)->at(indexAfter + SKIP));
//    m_cache->at(index)->replace(indexAfter + SKIP, temp);

//    int condition = indexAfter - indexBefore;
//    switch (condition) {
//    case 1:
//        //UP
//        setText(QObject::tr("Up local at %1").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(indexAfter).arg(indexBefore).arg(UNDOREDO_LV_SWAP));
//        break;
//    case -1:
//        //DOWN
//        setText(QObject::tr("Down local at %1").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(indexAfter).arg(indexBefore).arg(UNDOREDO_LV_SWAP));
//        break;
//    default:
//        //SWAP
//        setText(QObject::tr("Swap local at %1 %2 to %3").arg(temp.at(1)).arg(indexBefore).arg(indexAfter) \
//                + QString(" ^(%1,%2,%3)").arg(indexAfter).arg(indexBefore).arg(UNDOREDO_LV_SWAP));
//        break;
//    }

    replaceValue(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);
}

void SwapLocalVariantTree::redo()
{
//    QStringList temp;
//    temp = m_cache->at(index)->at(indexAfter + SKIP);

//    //swap
//    m_cache->at(index)->replace(indexAfter + SKIP
//                                        , m_cache->at(index)->at(indexBefore + SKIP));
//    m_cache->at(index)->replace(indexBefore + SKIP, temp);

//    int condition = indexAfter - indexBefore;

//    switch (condition) {
//    case 1:
//        //UP
//        setText(QObject::tr("Up local at %1").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UNDOREDO_LV_SWAP));
//        break;
//    case -1:
//        //DOWN
//        setText(QObject::tr("Down local at %1").arg(temp.at(1)) \
//                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UNDOREDO_LV_SWAP));
//        break;
//    default:
//        //SWAP
//        setText(QObject::tr("Swap local at %1 %2 to %3").arg(temp.at(1)).arg(indexBefore).arg(indexAfter) \
//                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UNDOREDO_LV_SWAP));
//        break;
//    }

    replaceValue(indexBefore, indexAfter);
    setTextByCondition(indexBefore, indexAfter);
}

int SwapLocalVariantTree::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(L_SWAP_TABLE);
}

void SwapLocalVariantTree::replaceValue(int source, int dest)
{
    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;
    list.swapItemsAt(source, dest);
    ec.local.variantData = list;
    ptrCache->replace(index, ec);
}

void SwapLocalVariantTree::setTextByCondition(int source, int dest)
{
    int condition = dest - source;
    QString arg = ptrCache->at(index).local.variantData.at(dest).variant;

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up local at %1").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_LV_SWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down local at %1").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_LV_SWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap local at %1 %2 to %3").arg(arg).arg(source).arg(dest) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UNDOREDO_LV_SWAP));
        break;
    }
}
