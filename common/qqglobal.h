#ifndef QQ_COMMON_GLOBAL_H
#define QQ_COMMON_GLOBAL_H

#include <qglobal.h>
#include <qtcpsocket.h>
#include <qudpsocket.h>
#include <qhostaddress.h>
#include <qsharedpointer.h>

#include "qqconfig.h"
#include "qqdefine.h"
#include "qqtheme.h"

namespace QQGlobals
{
    using namespace QQBaseConstValues;
    const QString AUDIO_FORMAT = "amr";
    const qreal AUDIO_MAX_TIME = 60;
    const qreal AUDIO_MIN_TIME = 3;

    Q_GLOBAL_STATIC(QQConfigs::UserConfig, g_user)
    Q_GLOBAL_STATIC(QQThemes::DefaultTheme, g_theme)
    Q_GLOBAL_STATIC(QTcpSocket, g_tcpSocket)
    Q_GLOBAL_STATIC(QUdpSocket, g_udpSocket)
    Q_GLOBAL_STATIC(QHostAddress, g_hostAddress)
    Q_GLOBAL_STATIC(quint16, g_hostPort)
}

#endif // QQ_COMMON_GLOBAL_H