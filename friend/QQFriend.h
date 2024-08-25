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

#include "common/qqwidget.h"

#include "ui_QQFriend.h"
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
	QQ_SINGLETON_CREATE(QQFriend)
public:
	explicit QQFriend(QWidget *parent = nullptr);
	~QQFriend();

protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watch, QEvent *event) override;

private:
	void loadPeopleInModelItem(QStandardItem *item, const QVariantMap &data);

	void handleStorge(const QByteArray &data);

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

private:
	Ui::QQFriendClass *ui;
	QProxyStyle *m_proxyStyle;
	ElaMenu *m_searchMoreMenu;
	QQFriendIndexDelegate *m_indexListDelegate;
	QStandardItemModel *m_friendsListModel;
	QStandardItemModel *m_groupsListModel;
	ElaMenu *m_friendContextMenu;
	QQWidgets::AnimationTabBar *m_tabBar;
	QMap<QString, QModelIndex> m_friendsGroups;
};

#endif // QQ_FRIEND_FRIEND_H