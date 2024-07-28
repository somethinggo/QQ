#include <qcoreapplication.h>
#include <qlocalserver.h>

static QLocalServer m_localServer;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    m_localServer.setObjectName("");
    m_localServer.listen("QQBackNetWorkHandleLocalClientServer");
    if (m_localServer.waitForNewConnection())
    {
    };

    return a.exec();
}
