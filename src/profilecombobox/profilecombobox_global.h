/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILECOMBOBOX_GLOBAL_H
#define PROFILECOMBOBOX_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROFILECOMBOBOX_LIBRARY)
#  define PROFILECOMBOBOXSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROFILECOMBOBOXSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROFILECOMBOBOX_GLOBAL_H
