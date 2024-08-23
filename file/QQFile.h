#ifndef QQ_FILE_FILE_H
#define QQ_FILE_FILE_H

#include <qglobal.h>

#include "common/qqdefine.h"

class QQFile
{
    QQ_SINGLETON_CREATE(QQFile)
public:
    QQFile();
    ~QQFile();
};

#endif // QQ_FILE_FILE_H