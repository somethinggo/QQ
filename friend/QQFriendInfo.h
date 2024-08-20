#ifndef QQ_FRIEND_INFO_H
#define QQ_FRIEND_INFO_H

#include <qwidget.h>
#include <qqueue.h>
#include <qlabel.h>
#include <qmap.h>
#include <qicon.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qflags.h>

#include "common/qqfunction.h"

class QQFriendInfo : public QWidget
{
	Q_OBJECT
	QQ_SINGLETON_CREATE(QQFriendInfo)
private:
	explicit QQFriendInfo(QWidget *parent = nullptr);
	~QQFriendInfo();

public:
	/**
	 * @brief 添加信息
	 * @param label 标签
	 * @param widget 控件
	 * @param icon 图标
	 * @param postion 位置
	 */
	void addInfo(QLabel *label, QWidget *widget, QIcon icon = QIcon(), QQEnums::InfoPostionType postion = QQEnums::InfoPostionType::left);

	/**
	 * @brief 添加信息
	 * @param name 名称
	 * @param widget 控件
	 * @param icon 图标
	 * @param postion 位置
	 */
	void addInfo(QString name, QWidget *widget, QIcon icon = QIcon(), QQEnums::InfoPostionType postion = QQEnums::InfoPostionType::left);

	/**
	 * @brief 获取信息
	 * @param index 索引
	 * @param type 类型
	 * @return 信息
	 */
	QWidget *getInfo(int index, QQEnums::InfoWidgetType type);

	/**
	 * @brief 获取信息
	 * @param index 索引
	 * @param types 类型
	 * @return 信息
	 */
	QList<QWidget *> getInfo(int index, QQEnums::InfoWidgetTypes types);

	/**
	 * @brief 移除信息
	 * @param index 索引
	 */
	void removeInfo(int index);

	/**
	 * @brief 移除所有信息
	 */
	void removeAllInfo();

private:
	void removeNextAllInfo(QLayout *layout);

private:
	QVBoxLayout *m_layout;
	QMap<int, QPair<QLayout *, QQEnums::InfoPostionType>> m_map; // 图标，名称，控件，布局，类型
};

#endif // QQ_FRIEND_INFO_H
