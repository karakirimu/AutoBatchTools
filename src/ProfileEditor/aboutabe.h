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

#ifndef ABOUTPE_H
#define ABOUTPE_H

#include <QWidget>

namespace Ui {
class AboutABE;
}

class AboutABE : public QWidget
{
    Q_OBJECT

public:
    explicit AboutABE(QWidget *parent = nullptr);
    ~AboutABE();

private:
    Ui::AboutABE *ui;
};

#endif // ABOUTPE_H