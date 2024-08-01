#pragma once

#include <qsharedpointer.h>
#include <qdatetime.h>

#ifdef DEBUG
#include <qdebug.h>
#endif

#include "qdefine.h"

namespace ClientConfigs
{
	Q_NAMESPACE

	// 本地化枚举值
	Q_MDECLARE_FLAGS(UserBaseConfig, UserStateType)
	Q_MDECLARE_FLAGS(GroupBaseConfig, GroupStateType)
	Q_MDECLARE_FLAGS(MessageBaseConfig, MessageType)
	Q_MDECLARE_FLAGS(DynamicBaseConfig, DynamicType)

	// 本地化友人配置
	typedef struct UserFriend_C : public UserBaseConfig
	{
		QString m_nikeName;
		uint64_t m_newMsgCount;
		QString m_lastMsg;
		QDateTime m_lastMsgTime;
		QString m_group;
		QList<QSharedPointer<ClientConfigs::MessageBaseConfig>> m_msgList;
	} UserFriend_C;

	// 本地化群聊配置
	typedef struct UserGroup_C : public GroupBaseConfig
	{
		QString m_nikeName;
		QString m_myNikeName;
		uint64_t m_newMsgCount;
		QString m_lastMsg;
		QDateTime m_lastMsgTime;
		QString m_group;
		QList<QSharedPointer<ClientConfigs::MessageBaseConfig>> m_msgList;
	} UserGroup_C;
}
// 向Qt元对象系统注册自定义类型
Q_DECLARE_METATYPE(ClientConfigs::UserFriend_C);
Q_DECLARE_METATYPE(ClientConfigs::UserGroup_C);
Q_DECLARE_METATYPE(ClientConfigs::MessageBaseConfig);
Q_DECLARE_METATYPE(ClientConfigs::UserFriend_C *);
Q_DECLARE_METATYPE(ClientConfigs::UserGroup_C *);
Q_DECLARE_METATYPE(ClientConfigs::MessageBaseConfig *);

namespace ClientEnums
{
	Q_NAMESPACE

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
