#ifndef QQ_CHAT_CHAT_H
#define QQ_CHAT_CHAT_H

#include <qwidget.h>
#include <qtextedit.h>
#include <qevent.h>
#include <qstandarditemmodel.h>
#include <qfiledialog.h>
#include <qdialogbuttonbox.h>
#include <qtextobject.h>
#include <qcombobox.h>
#include <qmetaobject.h>
#include <qfileiconprovider.h>
#include <qproxystyle.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <qtextcodec.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#elif Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#elif Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

#include <ElaMenu.h>
#include <ElaIcon.h>
#include <ElaMessageBar.h>

#include "common/qqwidget.h"
#include "audio/QQAudio.h"
#include "emoji/QQEmoji.h"

#include "ui_QQChat.h"
#include "QQChatIndexDelegate.h"
#include "QQChatMessageDelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QQChatClass;
};
QT_END_NAMESPACE

class QQChatProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
};

class QQChat : public QWidget
{
	Q_OBJECT
	QQ_SINGLETON_CREATE(QQChat)
public:
	explicit QQChat(QWidget *parent = nullptr);
	~QQChat();

protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watch, QEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	/**
	 * @brief 请求网络模块进行简单的非实时消息发送
	 * @param message 消息数据
	 */
	void sendMessageToNetWork(const QVariantMap &message);

	/**
	 * @brief 加载用户信息到聊天索引列表项中
	 * @param item 聊天索引列表项
	 * @param data 索引列表项数据
	 */
	void loadPeopleInModelItem(QStandardItem *item, const QVariantMap &data);

	/**
	 * @brief 加载消息信息到聊天消息列表项中
	 * @param item 聊天消息列表项
	 * @param data 消息数据
	 */
	void loadMessageInModelItem(QStandardItem *item, const QVariantMap &data);

	/**
	 * @brief 通过文件名发送文件
	 * @param fileName
	 * @note 本质不发送文件，而是先更新界面，然后发送文件名到网络模块，网络模块发送文件
	 */
	void sendFileByName(const QString &fileName);

	/**
	 * @brief 处理来自网络模块的消息
	 * @param data
	 */
	void handleNetwork(const QByteArray &data);

	/**
	 * @brief 处理来自存储模块的消息
	 * @param data
	 */
	void handleStorage(const QByteArray &data);

	/**
	 * @brief 处理来自表情模块的消息
	 * @param data
	 */
	void handleEmoji(const QByteArray &data);

	/**
	 * @brief 处理来自好友模块的消息
	 * @param data
	 */
	void handleFriend(const QByteArray &data);

	/**
	 * @brief 处理来自音频模块的消息
	 * @param data
	 */
	void handleAudio(const QByteArray &data);

private slots:
	void do_userClickSearchMore();
	void do_userClickSearchMoreAction(QAction *action);
	void do_userContextMenuAction(QAction *action);
	void do_userClickChatIndex(const QModelIndex &index);
	void do_userClickSendEmoji();
	void do_userClickSendFile();
	void do_userClickSendPicture();
	void do_userOpenBigWrite(bool isChecked);
	void do_userClickLookHistroy();
	void do_limitUserInputTextCount();
	void do_userClickSendAudio();
	void do_userClickClearInput();
	void do_userClickSendMessage();

private:
	const int MAX_SLIDE_SIZE = 5; // 最大显示消息数量
	Ui::QQChatClass *ui;
	QQChatProxyStyle *m_commonStyle;
	QAction *m_searchAction;
	ElaMenu *m_searchMoreMenu;
	QStandardItemModel *m_indexListModel;			 // 聊天索引列表数据模型
	QQChatIndexDelegate *m_indexListDelegate;		 // 聊天索引列表信息的样式代理
	QList<QStandardItem *> m_slide;					 // 检验哪些消息显示在界面上
	QStandardItemModel *m_messageListModel;			 // 聊天消息数据模型
	QQChatMessageDelegate *m_messageListDelegate;	 // 聊天消息的样式代理
	ElaMenu *m_chatFriendContextMenu;				 // 聊天索引列表的右键菜单--数据源是friend
	ElaMenu *m_chatGroupContextMenu;				 // 聊天索引列表的右键菜单--数据源是group
	QModelIndex m_activeChatIndex;					 // 当前活动的聊天索引--主要是为了控制消息的显示
	QMap<QString, QStandardItem *> m_haveInChatList; // 已经在聊天索引列表中的用户--主要是为了与友人界面的发送消息按钮进行关联
};

#endif // QQ_CHAT_CHAT_H