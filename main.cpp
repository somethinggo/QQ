#include <qapplication.h>

#include "three/include/ElaApplication.h"
#include "login/QQLogin.h"
#include "emoji/QQEmoji.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    QQEmoji::getInstance()->show();
    return a.exec();
}