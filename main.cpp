#include <qapplication.h>

#include "three/include/ElaApplication.h"

#include "QQMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    QQMain w;
    w.show();
    return a.exec();
}