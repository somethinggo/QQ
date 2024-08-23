#ifndef QQ_SYSTEM_SYSTEM_H
#define QQ_SYSTEM_SYSTEM_H

#include <qglobal.h>

#include "common/qqdefine.h"

class QQSystem
{
    QQ_SINGLETON_CREATE(QQSystem)
public:
    QQSystem();
    ~QQSystem();
};

#endif // QQ_SYSTEM_SYSTEM_H