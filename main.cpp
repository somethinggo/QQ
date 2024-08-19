#include <qapplication.h>

#include "three/include/ElaApplication.h"
#include "login/QQLogin.h"
#include "emoji/QQEmoji.h"
#include "audio/QQAudio.h"
#include "chat/QQChat.h"
#include "friend/QQFriend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    QQChat w;
    w.show();
    return a.exec();
}