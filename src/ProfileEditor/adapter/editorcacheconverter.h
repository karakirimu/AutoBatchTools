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

#ifndef EDITORCACHECONVERTER_H
#define EDITORCACHECONVERTER_H

#include "editorcache.h"
#include "../variantconverter/variantconverter.h"
#include <processxmlconstant.h>

class EditorCacheConverter : QObject
{
    Q_OBJECT
public:
    explicit EditorCacheConverter();
    ~EditorCacheConverter();

    void convertToEditorCache(const QList<QList<QStringList> *> *source, QList<EditorCache> *dest);
    void convertToXml(const QList<EditorCache> *source, QList<QList<QStringList> *> *dest);

    void convertFromCache(const EditorCache *from, QList<QStringList> *to);
    void convertToCache(EditorCache *to, const QList<QStringList> *from);

private:

    void fromInfomationCache(const EditorCache *from, QList<QStringList> *to);
    void fromLocalCache(const EditorCache *from, QList<QStringList> *to);
    void fromExecuteCache(const EditorCache *from, QList<QStringList> *to);
    void fromFileSearchCache(const EditorCache *from, QList<QStringList> *to);
    void fromPluginCache(const EditorCache *from, QList<QStringList> *to);
    void fromProfileLoadCache(const EditorCache *from, QList<QStringList> *to);

    void toInfomationCache(EditorCache *to, const QList<QStringList> *from);
    void toLocalCache(EditorCache *to, const QList<QStringList> *from);
    void toExecuteCache(EditorCache *to, const QList<QStringList> *from);
    void toFileSearchCache(EditorCache *to, const QList<QStringList> *from);
    void toPluginCache(EditorCache *to, const QList<QStringList> *from);
    void toProfileLoadCache(EditorCache *to, const QList<QStringList> *from);


    QString fetch(QString tag, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);
    QString fetch(QString tag, QString value, QString attr,const QList<QStringList> *loadbase);
    int fetchCommandFirstPos(QString tag, const QList<QStringList> *loadbase);

    FunctionType ft;
    ProcessXmlConstant pxc;
};

#endif // EDITORCACHECONVERTER_H
