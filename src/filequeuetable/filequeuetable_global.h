#ifndef FILEQUEUETABLE_GLOBAL_H
#define FILEQUEUETABLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FILEQUEUETABLE_LIBRARY)
#  define FILEQUEUETABLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILEQUEUETABLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FILEQUEUETABLE_GLOBAL_H
