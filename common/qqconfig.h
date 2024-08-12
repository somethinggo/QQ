#ifndef QQ_COMMON_CONFIG_H
#define QQ_COMMON_CONFIG_H

#include <qsharedpointer.h>
#include <qdatetime.h>

#ifdef DEBUG
#include <qdebug.h>
#endif

#include "qqtheme.h"
#include "qqenums.h"
#include "qqbase.h"

namespace QQConfigs
{
	Q_NAMESPACE

	// 本地化枚举值
	Q_MDECLARE_FLAGS(UserBaseConfig, UserStateType, QQBaseConfigs, QQConfigs)
	Q_MDECLARE_FLAGS(GroupBaseConfig, GroupStateType, QQBaseConfigs, QQConfigs)
	Q_MDECLARE_FLAGS(MessageBaseConfig, MessageType, QQBaseConfigs, QQConfigs)
	Q_MDECLARE_FLAGS(DynamicBaseConfig, DynamicType, QQBaseConfigs, QQConfigs)

	// 本地化友人配置
	typedef struct UserFriend_C : public UserBaseConfig
	{
		QString m_nikeName;
		uint64_t m_newMsgCount;
		QString m_lastMsg;
		QDateTime m_lastMsgTime;
		QString m_group;
		QList<QSharedPointer<QQConfigs::MessageBaseConfig>> m_msgList;
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
		QList<QSharedPointer<QQConfigs::MessageBaseConfig>> m_msgList;
	} UserGroup_C;
}
// 向Qt元对象系统注册自定义类型
Q_DECLARE_METATYPE(QQConfigs::UserFriend_C);
Q_DECLARE_METATYPE(QQConfigs::UserGroup_C);
Q_DECLARE_METATYPE(QQConfigs::MessageBaseConfig);
Q_DECLARE_METATYPE(QQConfigs::UserFriend_C *);
Q_DECLARE_METATYPE(QQConfigs::UserGroup_C *);
Q_DECLARE_METATYPE(QQConfigs::MessageBaseConfig *);

#endif