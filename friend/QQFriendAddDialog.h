#ifndef QQ_FRIEND_ADDFRIENDDIALOG_H
#define QQ_FRIEND_ADDFRIENDDIALOG_H

#include <qdialog.h>
#include <qlayout.h>
#include <qaction.h>
#include <qlabel.h>
#include <qdialogbuttonbox.h>
#include <qstackedwidget.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qsharedpointer.h>
#include <qstandarditemmodel.h>
#include <qpropertyanimation.h>

#include <ElaIcon.h>
#include <ElaLineEdit.h>

#include "common/qqwidget.h"
#include "common/qqglobal.h"
#include "QQFriendValidateInfo.h"

class QQFriendAddDialogProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQFriendAddDialog : public QDialog
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQFriendAddDialog)
private:
	explicit QQFriendAddDialog(QWidget *parent = nullptr);
	virtual ~QQFriendAddDialog();

public:
	/**
	 * @brief 添加用户信息
	 * @param user 用户信息
	 */
	void appendUser(QQConfigs::FriendConfig *user);
	/**
	 * @brief 添加群信息
	 * @param group 群信息
	 */
	void appendGroup(QQConfigs::GroupConfig *group);

private:
	QProxyStyle *m_proxyStyle;
	QStandardItemModel *m_usersModel;
	QStandardItemModel *m_groupsModel;
	QQWidgets::AppBar *m_appBar;
	ElaLineEdit *m_searchEdit;
	QQWidgets::AnimationTabBar *m_tabBar;
	QQFriendValidateInfo *m_validateInfo;
	QDialogButtonBox *m_buttonBox;
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

#endif // QQ_FRIEND_ADDFRIENDDIALOG_H
