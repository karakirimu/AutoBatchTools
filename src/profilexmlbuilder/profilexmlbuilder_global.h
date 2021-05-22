/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILEXMLBUILDER_GLOBAL_H
#define PROFILEXMLBUILDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROFILEXMLBUILDER_LIBRARY)
#  define PROFILEXMLBUILDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROFILEXMLBUILDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROFILEXMLBUILDER_GLOBAL_H
