/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SEARCHXMLBUILDER_GLOBAL_H
#define SEARCHXMLBUILDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SEARCHXMLBUILDER_LIBRARY)
#  define SEARCHXMLBUILDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SEARCHXMLBUILDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SEARCHXMLBUILDER_GLOBAL_H
