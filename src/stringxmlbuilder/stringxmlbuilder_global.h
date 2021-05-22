/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STRINGXMLBUILDER_GLOBAL_H
#define STRINGXMLBUILDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STRINGXMLBUILDER_LIBRARY)
#  define STRINGXMLBUILDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STRINGXMLBUILDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STRINGXMLBUILDER_GLOBAL_H
