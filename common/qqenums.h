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
    // 本地化枚举值
    Q_MDECLARE_FLAGS_BY_NAMESPACE(UserBaseConfig, UserStateType, QQBaseConfigs, QQEnums)
    Q_MDECLARE_FLAGS_BY_NAMESPACE(GroupBaseConfig, GroupStateType, QQBaseConfigs, QQEnums)
    Q_MDECLARE_FLAGS_BY_NAMESPACE(MessageBaseConfig, MessageType, QQBaseConfigs, QQEnums)
    Q_MDECLARE_FLAGS_BY_NAMESPACE(DynamicBaseConfig, DynamicType, QQBaseConfigs, QQEnums)

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
    };
    Q_MREGISTER_EVENT(login, static_cast<int>(EventType::login))
    Q_MREGISTER_EVENT(logout, static_cast<int>(EventType::logout))
    Q_MREGISTER_EVENT(registered, static_cast<int>(EventType::registered))
    Q_MREGISTER_EVENT(findpassword, static_cast<int>(EventType::findpassword))
    Q_MREGISTER_EVENT(accpectlogin, static_cast<int>(EventType::accpectlogin))
    Q_MREGISTER_EVENT(accpectlogout, static_cast<int>(EventType::accpectlogout))
    Q_MREGISTER_EVENT(accpectregistered, static_cast<int>(EventType::accpectregistered))
    Q_MREGISTER_EVENT(accpectfindpassword, static_cast<int>(EventType::accpectfindpassword))
    Q_MREGISTER_EVENT(sendtext, static_cast<int>(EventType::sendtext))
    Q_MREGISTER_EVENT(sendimage, static_cast<int>(EventType::sendimage))
    Q_MREGISTER_EVENT(sendlink, static_cast<int>(EventType::sendlink))
    Q_MREGISTER_EVENT(sendemoji, static_cast<int>(EventType::sendemoji))
    Q_MREGISTER_EVENT(sendaudio, static_cast<int>(EventType::sendaudio))
    Q_MREGISTER_EVENT(sendfile, static_cast<int>(EventType::sendfile))
    Q_MREGISTER_EVENT(sendreport, static_cast<int>(EventType::sendreport))
    Q_MREGISTER_EVENT(searchfriend, static_cast<int>(EventType::searchfriend))
    Q_MREGISTER_EVENT(searchgroup, static_cast<int>(EventType::searchgroup))
    Q_MREGISTER_EVENT(addfriend, static_cast<int>(EventType::addfriend))
    Q_MREGISTER_EVENT(addgroup, static_cast<int>(EventType::addgroup))
    Q_MREGISTER_EVENT(addemoji, static_cast<int>(EventType::addemoji))
    Q_MREGISTER_EVENT(loademoji, static_cast<int>(EventType::loademoji))
    Q_MREGISTER_EVENT(loadmessage, static_cast<int>(EventType::loadmessage))
    Q_MREGISTER_EVENT(loadset, static_cast<int>(EventType::loadset))
    Q_MREGISTER_EVENT(requestaudio, static_cast<int>(EventType::requestaudio))
    Q_MREGISTER_EVENT(requestfile, static_cast<int>(EventType::requestfile))

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
    Q_MDECLARE_OPERATORS_FOR_FLAGS(RoundPostionTypes)

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
    Q_MDECLARE_OPERATORS_FOR_FLAGS(FileSizeTypes)

    // 信息的位置枚举
    enum class InformationPostionType : char
    {
        left,
        bottom
    };
    // 信息的类型枚举
    enum class InformationWidgetType : char
    {
        icon = 0x01,
        label = 0x02,
        widget = 0x04,
        all = icon | label | widget
    };
    Q_DECLARE_FLAGS(InformationWidgetTypes, InformationWidgetType)
    Q_MDECLARE_OPERATORS_FOR_FLAGS(InformationWidgetTypes)

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
    Q_MDECLARE_OPERATORS_FOR_FLAGS(AppBarHintTypes)

    // 表情的类型枚举
    enum class EmojiType : char
    {
        unicode,
        picture,
        gif
    };
    Q_DECLARE_FLAGS(EmojiTypes, EmojiType)
    Q_MDECLARE_OPERATORS_FOR_FLAGS(EmojiTypes)

    // 索引的类型枚举
    enum class PeopleWidgetType : char
    {
        friendchats,
        groupchats,
        friends,
        groups,
        friendappend,
        groupappend
    };
    Q_ENUM_NS(PeopleWidgetType)
}

#endif