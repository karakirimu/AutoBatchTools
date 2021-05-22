/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QLineEdit>
#include <multitask.h>
#include <QKeyEvent>

class ConsoleEdit : public QLineEdit
{
public:
    explicit ConsoleEdit(QWidget *parent = nullptr);
    ~ConsoleEdit() override;

    void setMultiTask(MultiTask *mltask);

protected:
    void keyPressEvent(QKeyEvent *keyevent) override;

private slots:
    void sendMessage();

private:
    MultiTask *mlTask;

};

#endif // CONSOLEEDIT_H
