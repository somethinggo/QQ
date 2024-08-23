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

    // 全局用户配置
    Q_GLOBAL_STATIC(QQConfigs::UserConfig, g_user)

    // 全局主题配置
    Q_GLOBAL_STATIC(QQThemes::DefaultTheme, g_theme)

    // 全局TCP套接字
    Q_GLOBAL_STATIC(QTcpSocket, g_tcpSocket)

    // 全局UDP套接字
    Q_GLOBAL_STATIC(QUdpSocket, g_udpSocket)

    // 全局主机地址
    Q_GLOBAL_STATIC(QHostAddress, g_hostAddress)

    // 全局主机端口
    Q_GLOBAL_STATIC(quint16, g_hostPort)

    // 全局单例管理
    Q_GLOBAL_STATIC(QList<QObject *>, g_singletonManager)

    // 全局协议版本号
    Q_GLOBAL_STATIC(QString, g_version)
}

#endif // QQ_COMMON_GLOBAL_H