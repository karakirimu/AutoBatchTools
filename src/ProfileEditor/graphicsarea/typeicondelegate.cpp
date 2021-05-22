/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "typeicondelegate.h"


TypeIconDelegate::TypeIconDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

void TypeIconDelegate::paint(QPainter *painter \
                             , const QStyleOptionViewItem &option \
                             , const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if(index.data(Qt::UserRole).canConvert<QIcon>()){
        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::UserRole));
        painter->save();
        icon.paint(painter, option.rect);
        painter->restore();
    }
}
