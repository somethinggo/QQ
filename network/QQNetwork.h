#ifndef QQ_NETWORK_NETWORK_H
#define QQ_NETWORK_NETWORK_H

#include <qglobal.h>

#include "common/qqdefine.h"

class QQNetwork
{
    QQ_SINGLETON_CREATE(QQNetwork)

public:
    QQNetwork();
    ~QQNetwork();
};

#endif // QQ_NETWORK_NETWORK_H