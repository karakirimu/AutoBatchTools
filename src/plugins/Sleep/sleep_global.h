#ifndef SLEEP_GLOBAL_H
#define SLEEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SLEEP_LIBRARY)
#  define SLEEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SLEEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SLEEP_GLOBAL_H
