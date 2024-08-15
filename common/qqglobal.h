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

    extern QSharedPointer<QQConfigs::UserConfig> g_user;
    extern QSharedPointer<QTcpSocket> g_tcpSocket;
    extern QSharedPointer<QUdpSocket> g_udpSocket;
    extern QHostAddress g_hostAddress;
    extern quint16 g_hostPort;
    extern QSharedPointer<QQThemes::DefaultTheme> g_theme;
}

#endif // QQ_COMMON_GLOBAL_H