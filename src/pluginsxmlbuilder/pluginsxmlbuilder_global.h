/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PLUGINSXMLBUILDER_GLOBAL_H
#define PLUGINSXMLBUILDER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGINSXMLBUILDER_LIBRARY)
#  define PLUGINSXMLBUILDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGINSXMLBUILDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINSXMLBUILDER_GLOBAL_H
