/*
 * Copyright 2016-2019 karakirimu
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

#ifndef STRINGTABLE_GLOBAL_H
#define STRINGTABLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STRINGTABLE_LIBRARY)
#  define STRINGTABLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define STRINGTABLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STRINGTABLE_GLOBAL_H
