/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EXECUTOR_GLOBAL_H
#define EXECUTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXECUTOR_LIBRARY)
#  define EXECUTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXECUTORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EXECUTOR_GLOBAL_H
