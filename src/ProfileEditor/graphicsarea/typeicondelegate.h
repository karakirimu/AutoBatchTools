/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef TYPEICONDELEGATE_H
#define TYPEICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class TypeIconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TypeIconDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

};

#endif // TYPEICONDELEGATE_H
