/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ABOUTABS_H
#define ABOUTABS_H

#include <QWidget>

namespace Ui {
class AboutABS;
}

class AboutABS : public QWidget
{
    Q_OBJECT

public:
    explicit AboutABS(QWidget *parent = nullptr);
    ~AboutABS();

private:
    Ui::AboutABS *ui;
};

#endif // ABOUTPS_H
