#include <qapplication.h>

#include "three/include/ElaApplication.h"
#include "login/QQLogin.h"
#include "emoji/QQEmoji.h"
#include "audio/QQAudio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    QQAudio::getInstance()->show();
    return a.exec();
}