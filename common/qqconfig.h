#ifndef QQ_COMMON_CONFIG_H
#define QQ_COMMON_CONFIG_H

#include <qsharedpointer.h>
#include <qdatetime.h>

#ifndef DEBUG
#include <qdebug.h>
#endif

#include "qqbase.h"

namespace QQConfigs
{
	Q_NAMESPACE

	typedef QQBaseConfigs::UserBaseConfig UserConfig;
	typedef QQBaseConfigs::MessageBaseConfig MessageConfig;

	// 本地化友人配置
	typedef struct FriendConfig : public UserConfig
	{
		QString m_nickname;
		uint64_t m_count;
		QString m_message;
		QDateTime m_time;
		QString m_group;
		QList<QSharedPointer<MessageConfig>> m_msgList;
	} FriendConfig;

	// 本地化群聊配置
	typedef struct GroupConfig : public QQBaseConfigs::GroupBaseConfig
	{
		QString m_nickname;
		QString m_byname;
		uint64_t m_count;
		QString m_message;
		QDateTime m_time;
		QString m_group;
		QList<QSharedPointer<MessageConfig>> m_msgList;
	} GroupConfig;

}
// 向Qt元对象系统注册自定义类型
Q_DECLARE_METATYPE(QQConfigs::FriendConfig);
Q_DECLARE_METATYPE(QQConfigs::GroupConfig);
Q_DECLARE_METATYPE(QQConfigs::MessageConfig);
Q_DECLARE_METATYPE(QQConfigs::FriendConfig *);
Q_DECLARE_METATYPE(QQConfigs::GroupConfig *);
Q_DECLARE_METATYPE(QQConfigs::MessageConfig *);

#endif