/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
