#include "qglobalvalue.h"

QLocalSocket GlobalValuesAgent::m_localSocket;
BaseConfigs::UserBaseConfig GlobalValuesAgent::m_user;
QList<QObject *> GlobalValuesAgent::m_singletons;
QQueue<QThread *> GlobalValuesAgent::m_threadPool;