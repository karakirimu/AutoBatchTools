/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef XMLBUILDER_GLOBAL_H
#define XMLBUILDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLBUILDER_LIBRARY)
#  define XMLBUILDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLBUILDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLBUILDER_GLOBAL_H
