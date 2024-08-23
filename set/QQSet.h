#ifndef QQ_SET_SET_H
#define QQ_SET_SET_H

#include <qglobal.h>

#include "common/qqdefine.h"

class QQSet
{
    QQ_SINGLETON_CREATE(QQSet)
public:
    QQSet();
    ~QQSet();
};

#endif // QQ_SET_SET_H