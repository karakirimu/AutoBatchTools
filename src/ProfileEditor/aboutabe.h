/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ABOUTPE_H
#define ABOUTPE_H

#include <QWidget>
#include <QIcon>

namespace Ui {
class AboutABE;
}

class AboutABE : public QWidget
{
    Q_OBJECT

public:
    explicit AboutABE(QWidget *parent = nullptr);
    ~AboutABE() override;

private:
    Ui::AboutABE *ui;
};

#endif // ABOUTPE_H
