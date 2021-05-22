/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBBASICTABLE_GLOBAL_H
#define LIBBASICTABLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASICTABLE_LIBRARY)
#  define BASICTABLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASICTABLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBBASICTABLE_GLOBAL_H
