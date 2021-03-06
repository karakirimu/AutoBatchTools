/*
 * Copyright 2016-2021 karakirimu
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

#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QKeyEvent>
#include <QLineEdit>
#include <multitaskp.h>

class ConsoleEdit : public QLineEdit
{
public:
    explicit ConsoleEdit(QWidget *parent = nullptr);
    ~ConsoleEdit() override;

    void setMultiTask(MultiTaskP *rbinder);

protected:
    void keyPressEvent(QKeyEvent *keyevent) override;

private slots:
    void sendMessage();

private:
    MultiTaskP *binder;

};

#endif // CONSOLEEDIT_H
