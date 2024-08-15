#include "qqglobal.h"

using namespace QQGlobals;

QSharedPointer<QQConfigs::UserConfig> QQGlobals::g_user = QSharedPointer<QQConfigs::UserConfig>(new QQConfigs::UserConfig);
QSharedPointer<QQThemes::DefaultTheme> QQGlobals::g_theme = QSharedPointer<QQThemes::DefaultTheme>(new QQThemes::DefaultTheme);
QSharedPointer<QTcpSocket> QQGlobals::g_tcpSocket = QSharedPointer<QTcpSocket>(new QTcpSocket());
QSharedPointer<QUdpSocket> QQGlobals::g_udpSocket = QSharedPointer<QUdpSocket>(new QUdpSocket());
QHostAddress QQGlobals::g_hostAddress = QHostAddress();
quint16 QQGlobals::g_hostPort = 0;