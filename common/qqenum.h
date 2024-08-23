#ifndef QQ_COMMON_ENUM_H
#define QQ_COMMON_ENUM_H

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
        senduserinfo,
        receiveuserinfo,
        sendmessage,
        receivemessage,
        searchinfo,
        addinfo,
        receiveinfo,
        loadstorage,
        receivestorage,
        savestorage,
        requestmoudel,
        statechange,
    };
    QQ_REGISTER_EVENT(senduserinfo, static_cast<int>(EventType::senduserinfo))
    QQ_REGISTER_EVENT(receiveuserinfo, static_cast<int>(EventType::receiveuserinfo))
    QQ_REGISTER_EVENT(sendmessage, static_cast<int>(EventType::sendmessage))
    QQ_REGISTER_EVENT(receivemessage, static_cast<int>(EventType::receivemessage))
    QQ_REGISTER_EVENT(searchinfo, static_cast<int>(EventType::searchinfo))
    QQ_REGISTER_EVENT(addinfo, static_cast<int>(EventType::addinfo))
    QQ_REGISTER_EVENT(receiveinfo, static_cast<int>(EventType::receiveinfo))
    QQ_REGISTER_EVENT(loadstorage, static_cast<int>(EventType::loadstorage))
    QQ_REGISTER_EVENT(receivestorage, static_cast<int>(EventType::receivestorage))
    QQ_REGISTER_EVENT(savestorage, static_cast<int>(EventType::savestorage))
    QQ_REGISTER_EVENT(requestmoudel, static_cast<int>(EventType::requestmoudel))
    QQ_REGISTER_EVENT(statechange, static_cast<int>(EventType::statechange))

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