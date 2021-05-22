/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SEARCHTABLE_GLOBAL_H
#define SEARCHTABLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SEARCHTABLE_LIBRARY)
#  define SEARCHTABLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SEARCHTABLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SEARCHTABLE_GLOBAL_H
