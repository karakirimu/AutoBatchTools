/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef FILESEARCHLOADER_GLOBAL_H
#define FILESEARCHLOADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FILESEARCHLOADER_LIBRARY)
#  define FILESEARCHLOADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILESEARCHLOADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FILESEARCHLOADER_GLOBAL_H
