/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ABOUTPR_H
#define ABOUTPR_H

#include <QWidget>

namespace Ui {
class AboutABR;
}

class AboutABR : public QWidget
{
    Q_OBJECT

public:
    explicit AboutABR(QWidget *parent = nullptr);
    ~AboutABR();

private:
    Ui::AboutABR *ui;
};

#endif // ABOUTPR_H
