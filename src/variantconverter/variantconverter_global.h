/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTCONVERTER_GLOBAL_H
#define VARIANTCONVERTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VARIANTCONVERTER_LIBRARY)
#  define VARIANTCONVERTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VARIANTCONVERTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VARIANTCONVERTER_GLOBAL_H
