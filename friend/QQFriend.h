#ifndef QQ_FRIEND_FRIEND_H
#define QQ_FRIEND_FRIEND_H

#include <qwidget.h>
#include <qevent.h>
#include <qtimer.h>
#include <qstandarditemmodel.h>
#include <qtextobject.h>

#include <ElaIcon.h>
#include <ElaMenu.h>
#include <ElaLineEdit.h>
#include <ElaIconButton.h>
#include <ElaComboBox.h>
#include <ElaMessageBar.h>

#include "ui_QQFriend.h"
#include "common/qqwidget.h"
#include "common/qqglobal.h"
#include "QQFriendIndexDelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QQFriendClass;
};
QT_END_NAMESPACE

class QQFriendProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQFriend : public QWidget
{
	Q_OBJECT

public:
	explicit QQFriend(QWidget *parent = nullptr);
	virtual ~QQFriend();

private:
	Ui::QQFriendClass *ui;
	QProxyStyle *m_proxyStyle;
	ElaMenu *m_searchMoreMenu;

	/*
	聊天索引列表模型,好友列表模型,群组列表模型--数据设置(以Qt::UserRole为基准)
	0:索引体指针
	1:索引窗口类型
	2:索引体内部数据头像
	3:索引体内部数据名称
	--好友聊天显示好友名称--群组聊天显示群名称
	--好友信息显示好友名称+备注--群组列表显示群名称+备注
	--其它显示名称--有备注显示备注(备注未显示在索引窗口)
	//根据索引窗口类型设置
	4:文本内容--好友聊天显示最新消息--好友列表显示好友签名--群组聊天显示最新消息--群组列表无该项--其它显示账号
	5:消息时间--好友聊天显示最新消息时间--群组显示最新消息时间--其它无该项
	6:未读消息数量--好友聊天显示未读消息数量--群组显示未读消息数量--其它无该项
	7:免打扰设置--好友聊天显示免打扰设置--群组显示免打扰设置--其它无该项
	*/
	QQFriendIndexDelegate *m_indexListDelegate;
	QStandardItemModel *m_friendsListModel;
	QStandardItemModel *m_groupsListModel;
	ElaMenu *m_friendContextMenu;
	QQWidgets::AnimationTabBar *m_tabBar;
	QMap<QString, QModelIndex> m_friendsGroups;

protected:
	bool eventFilter(QObject *watch, QEvent *event) override;

private:
	void loadPeopleInModelItem(QStandardItem *item, const QQConfigs::FriendConfig &user);
	void loadPeopleInModelItem(QStandardItem *item, const QQConfigs::GroupConfig &group);
private slots:
	void do_userClickSearchMore();
	void do_userClickSearchMoreAction(QAction *action);
	void do_userClickRightContextMenu(const QPoint &pos);
	void do_userClickRightContextMenuAction(QAction *action);
	void do_userClickNotice();
	void do_userClickFriendListIndex(const QModelIndex &index);
	void do_userClickGroupListIndex(const QModelIndex &index);
	void do_userSelectedGroupChanged(const QString &text);
	void do_userClickShareOther();
	void do_userClickMusicConnect();
	void do_userClickSendMessage();
};

#endif // QQ_FRIEND_FRIEND_H