#ifndef QGLOBALVALUE_H
#define QGLOBALVALUE_H

#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <qtcpsocket.h>
#include <qudpsocket.h>
#include <qlist.h>

extern QString m_localQQLoginWidgetPath;        // 本地QQ登录窗口路径
extern QString m_localQQDesktopPath;            // 本地QQ主窗口路径
extern QLocalServer m_localServer;              // 与本地客户端通信的服务器
extern QList<QLocalSocket *> m_localSocketList; // 与本地客户端通信的套接字列表
extern bool m_isConnectToServer;                // 本地服务器是否已经监听
extern QTcpSocket m_tcpSocket;                  // 与远程客户端通信的套接字--长连接
extern QUdpSocket m_udpSocket;                  // 与远程客户端通信的套接字--短连接
extern QString m_remoteServerIp;                // 远程服务器的IP地址
extern quint16 m_remoteServerPort;              // 远程服务器的端口号
extern quint64 m_readBufferSize;                // 读缓冲区大小

#endif