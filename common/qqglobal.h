#ifndef QQ_COMMON_GLOBAL_H
#define QQ_COMMON_GLOBAL_H

#include <qglobal.h>
#include <qtcpsocket.h>
#include <qudpsocket.h>
#include <qhostaddress.h>
#include <qsharedpointer.h>

#include "qqtheme.h"

namespace QQGlobals
{
    QSharedPointer<QTcpSocket> g_tcpSocket;
    QSharedPointer<QUdpSocket> g_udpSocket;
    QHostAddress g_hostAddress;
    quint16 g_hostPort;
    QSharedPointer<QQThemes::DefaultTheme> g_theme;
}

#endif // QQ_COMMON_GLOBAL_H