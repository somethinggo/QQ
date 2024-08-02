#include <qcoreapplication.h>
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <functional>
#include "QQHandleMessage.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QProcess::startDetached(m_localQQLoginWidgetPath);
    m_localServer.listen("QQBackNetWorkHandleLocalClientServer");
    while (m_localServer.waitForNewConnection(3000))
    {
        QLocalSocket *socket = m_localServer.nextPendingConnection();
        m_localSocketList.append(socket);
        socket->setReadBufferSize(m_readBufferSize);
        QObject::connect(socket, &QLocalSocket::readyRead, [=]()
                         { QQHandleMessageFactory::createCalculator("v1")->handleLocalClientMessage(*socket); });
    }
    return a.exec();
}
