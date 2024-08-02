#include "qglobalvalue.h"

QString m_localQQLoginWidgetPath = "C:/Users/30842/Desktop/QQ/bin/win/QQLoginWidget.exe";
QString m_localQQDesktopPath = "C:/Users/30842/Desktop/QQ/bin/win/QQDesktop.exe";
QLocalServer m_localServer;
QList<QLocalSocket *> m_localSocketList;
bool m_isConnectToServer = false;
QTcpSocket m_tcpSocket;
QUdpSocket m_udpSocket;
QString m_remoteServerIp = "";
quint16 m_remoteServerPort = 1234;
quint64 m_readBufferSize = 16 * 1024;