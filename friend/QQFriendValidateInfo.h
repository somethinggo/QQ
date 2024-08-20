#ifndef QQ_FRIEND_VALIDATEINFO_H
#define QQ_FRIEND_VALIDATEINFO_H

#include <qwidget.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <qlayout.h>
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>
#include <qproxystyle.h>
#include <qstyleoption.h>

#include "common/qqfunction.h"

class QQFriendValidateInfo : public QWidget
{
	Q_OBJECT
	QQ_SINGLETON_CREATE(QQFriendValidateInfo)
private:
	explicit QQFriendValidateInfo(QWidget *parent = nullptr);
	~QQFriendValidateInfo();

public:
	/**
	 * @brief 设置基本信息
	 * @param user 用户信息
	 */
	void setBasePeople(QQConfigs::FriendConfig *user);

	/**
	 * @brief 设置基本信息
	 * @param group 群信息
	 */
	void setBaseGroup(QQConfigs::GroupConfig *group);

	/**
	 * @brief 获取基本信息
	 * @return 基本信息
	 */
	QPair<QQConfigs::FriendConfig *, QQConfigs::GroupConfig *> getBaseInfo();

	/**
	 * @brief 添加验证信息
	 * @param label 标签
	 * @param widget 控件
	 */
	void addValidate(QLabel *label, QWidget *widget);

	/**
	 * @brief 添加验证信息
	 * @param name 名称
	 * @param widget 控件
	 */
	void addValidate(QString name, QWidget *widget);

	/**
	 * @brief 获取验证信息
	 * @param index 索引
	 * @return 验证信息
	 */
	QPair<QLabel *, QWidget *> getValidate(int index);

	/**
	 * @brief 移除验证信息
	 * @param index 索引
	 */
	void removeValidate(int index);

	/**
	 * @brief 移除所有验证信息
	 */
	void removeAllValidate();

private:
	QQConfigs::FriendConfig *m_user;
	QQConfigs::GroupConfig *m_group;
	QWidget *m_baseInfo;
	QScrollArea *m_validateList;
	QMap<int, QPair<QLabel *, QWidget *>> m_map;
};

#endif // QQ_FRIEND_VALIDATEINFO_H