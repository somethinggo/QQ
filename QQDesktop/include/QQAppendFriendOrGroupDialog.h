#pragma once
// 添加好友或是群组界面
#include <qdialog.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qaction.h>
#include <qlineedit.h>
#include <qpropertyanimation.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qsharedpointer.h>

#include <ElaIcon.h>

#include "qfunction.hpp"
#include "QQAppBar.h"
#include "QQActionTabBar.h"
#include "QQCustomDelegate.h"
#include "QQScrollerPixelListView.h"
#include "QQAppendNeedInformationWidget.h"

class QQAppendFriendOrGroupDialogProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQAppendFriendOrGroupDialog : public QDialog
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQAppendFriendOrGroupDialog)
private:
	explicit QQAppendFriendOrGroupDialog(QWidget *parent = nullptr);
	virtual ~QQAppendFriendOrGroupDialog();

public:
	void appendUser(ClientConfigs::UserFriend_C *user);
	void appendGroup(ClientConfigs::UserGroup_C *group);

private:
	QQAppendFriendOrGroupDialogProxyStyle *m_proxyStyle;
	QStandardItemModel *m_usersModel;
	QStandardItemModel *m_groupsModel;
	QQSinglePeopleWidgetDelegate *m_friendAndGroupDelegate;
	QQAppBar *m_appBar;
	QLineEdit *m_searchEdit;
	QQActionTabBar *m_tabBar;
	QStackedWidget *m_stackedWidget;
	QStackedWidget *m_informationWidget;

protected:
	void closeEvent(QCloseEvent *event) override;
private slots:
	void do_tabBarCurrentChanged(int index);
	void do_searchTextChanged(const QString &text);
	void do_userClickFriendItem(const QModelIndex &index);
	void do_userClickGroupItem(const QModelIndex &index);
};
