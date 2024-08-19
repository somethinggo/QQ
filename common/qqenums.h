#ifndef QQ_COMMON_ENUMS_H
#define QQ_COMMON_ENUMS_H

#include <qglobal.h>
#include <qnamespace.h>
#include <qflags.h>

#include "qqbase.h"
#include "qqdefine.h"

namespace QQEnums
{
    Q_NAMESPACE
    using namespace QQBaseConfigs;
    // 本地化枚举值
    QQ_DECLARE_FLAGS(UserBaseConfig, UserStateType)
    QQ_DECLARE_FLAGS(GroupBaseConfig, GroupStateType)
    QQ_DECLARE_FLAGS(MessageBaseConfig, MessageType)
    QQ_DECLARE_FLAGS(DynamicBaseConfig, DynamicType)
    // 事件枚举
    enum class EventType
    {
        login,
        logout,
        registered,
        findpassword,
        accpectlogin,
        accpectlogout,
        accpectregistered,
        accpectfindpassword,
        sendtext,
        sendimage,
        sendlink,
        sendemoji,
        sendaudio,
        sendfile,
        sendreport,
        searchfriend,
        searchgroup,
        addfriend,
        addgroup,
        addemoji,
        loademoji,
        loadmessage,
        loadset,
        requestaudio,
        requestfile,
        requestchat,
    };
    QQ_REGISTER_EVENT(login, static_cast<int>(EventType::login))
    QQ_REGISTER_EVENT(logout, static_cast<int>(EventType::logout))
    QQ_REGISTER_EVENT(registered, static_cast<int>(EventType::registered))
    QQ_REGISTER_EVENT(findpassword, static_cast<int>(EventType::findpassword))
    QQ_REGISTER_EVENT(accpectlogin, static_cast<int>(EventType::accpectlogin))
    QQ_REGISTER_EVENT(accpectlogout, static_cast<int>(EventType::accpectlogout))
    QQ_REGISTER_EVENT(accpectregistered, static_cast<int>(EventType::accpectregistered))
    QQ_REGISTER_EVENT(accpectfindpassword, static_cast<int>(EventType::accpectfindpassword))
    QQ_REGISTER_EVENT(sendtext, static_cast<int>(EventType::sendtext))
    QQ_REGISTER_EVENT(sendimage, static_cast<int>(EventType::sendimage))
    QQ_REGISTER_EVENT(sendlink, static_cast<int>(EventType::sendlink))
    QQ_REGISTER_EVENT(sendemoji, static_cast<int>(EventType::sendemoji))
    QQ_REGISTER_EVENT(sendaudio, static_cast<int>(EventType::sendaudio))
    QQ_REGISTER_EVENT(sendfile, static_cast<int>(EventType::sendfile))
    QQ_REGISTER_EVENT(sendreport, static_cast<int>(EventType::sendreport))
    QQ_REGISTER_EVENT(searchfriend, static_cast<int>(EventType::searchfriend))
    QQ_REGISTER_EVENT(searchgroup, static_cast<int>(EventType::searchgroup))
    QQ_REGISTER_EVENT(addfriend, static_cast<int>(EventType::addfriend))
    QQ_REGISTER_EVENT(addgroup, static_cast<int>(EventType::addgroup))
    QQ_REGISTER_EVENT(addemoji, static_cast<int>(EventType::addemoji))
    QQ_REGISTER_EVENT(loademoji, static_cast<int>(EventType::loademoji))
    QQ_REGISTER_EVENT(loadmessage, static_cast<int>(EventType::loadmessage))
    QQ_REGISTER_EVENT(loadset, static_cast<int>(EventType::loadset))
    QQ_REGISTER_EVENT(requestaudio, static_cast<int>(EventType::requestaudio))
    QQ_REGISTER_EVENT(requestfile, static_cast<int>(EventType::requestfile))
    QQ_REGISTER_EVENT(requestchat, static_cast<int>(EventType::requestchat))

    // 方向枚举
    enum class DirectionType : char
    {
        lefttoright,
        righttoleft,
    };

    // 蒙版的方向枚举
    enum class RoundPostionType : char
    {
        topleft = 0x01,
        topright = 0x02,
        bottomleft = 0x04,
        bottomright = 0x08,
        top = topleft | topright,
        bottom = bottomleft | bottomright,
        left = topleft | bottomleft,
        right = topright | bottomright,
        all = top | bottom,
    };
    Q_DECLARE_FLAGS(RoundPostionTypes, RoundPostionType)
    QQ_DECLARE_OPERATORS_FOR_FLAGS(RoundPostionTypes)

    // 文件的大小单位枚举
    enum class FileSizeType : char
    {
        B,
        KB,
        MB,
        GB,
        TB
    };
    Q_DECLARE_FLAGS(FileSizeTypes, FileSizeType)
    QQ_DECLARE_OPERATORS_FOR_FLAGS(FileSizeTypes)

    // 信息的位置枚举
    enum class InfoPostionType : char
    {
        left,
        bottom
    };
    // 信息的类型枚举
    enum class InfoWidgetType : char
    {
        icon = 0x01,
        label = 0x02,
        widget = 0x04,
        all = icon | label | widget
    };
    Q_DECLARE_FLAGS(InfoWidgetTypes, InfoWidgetType)
    QQ_DECLARE_OPERATORS_FOR_FLAGS(InfoWidgetTypes)

    // 标题栏的类型枚举
    enum class AppBarHintType : char
    {
        NoButtonHint = 0x00,
        MinimizeButtonHint = 0x01,
        MaximizeButtonHint = 0x02,
        CloseButtonHint = 0x04,
        WindowsHint = MinimizeButtonHint | MaximizeButtonHint | CloseButtonHint,
        DialogHint = CloseButtonHint
    };
    Q_DECLARE_FLAGS(AppBarHintTypes, AppBarHintType)
    QQ_DECLARE_OPERATORS_FOR_FLAGS(AppBarHintTypes)

    // 表情的类型枚举
    enum class EmojiType : char
    {
        unicode,
        picture,
        gif
    };
    Q_DECLARE_FLAGS(EmojiTypes, EmojiType)
    QQ_DECLARE_OPERATORS_FOR_FLAGS(EmojiTypes)
}

#endif