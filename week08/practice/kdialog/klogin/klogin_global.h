#ifndef KLOGIN_GLOBAL_H
#define KLOGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(KLOGIN_LIBRARY)
#  define KLOGIN_EXPORT Q_DECL_EXPORT
#else
#  define KLOGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // KLOGIN_GLOBAL_H