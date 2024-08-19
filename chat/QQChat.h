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

#include "ui_QQChat.h"
#include "qqwidget.h"
#include "QQChatIndexDelegate.h"
#include "QQChatMessageDelegate.h"
#include "audio/QQAudio.h"
#include "emoji/QQEmoji.h"

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
public:
	explicit QQChat(QWidget *parent = nullptr);
	virtual ~QQChat();

private:
	Ui::QQChatClass *ui;
	QQChatProxyStyle *m_commonStyle;
	QAction *m_searchAction;
	ElaMenu *m_searchMoreMenu;

	/*
	聊天索引列表模型,好友列表模型,群组列表模型--数据设置(以Qt::UserRole为基准)
	0:索引体指针
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
	QStandardItemModel *m_indexListModel;	  // 聊天索引列表数据模型
	QQChatIndexDelegate *m_indexListDelegate; // 聊天索引列表信息的样式代理
	/*
	消息模型--数据设置(以Qt::UserRole为基准)
	0:消息体指针
	1:发送者头像
	2:发送者名称--好友聊天显示好友名称--群组显示群名称--有备注显示备注
	3:文本内容--文本消息显示文本内容--文件消息显示文件大小--链接消息显示链接地址
	4:图片内容--图片消息显示图片内容--文件消息显示文件图标
	//状态信息
	5:发送状态--发送中,发送成功,发送失败
	--本地用户发送的消息默认发送失败到发送中,发送成功后设置为发送成功,发送失败后设置为发送失败
	--系统消息与时间消息只有发送成功状态(不会向后台发送消息,存储在本地)
	--界面只显示发送中,发送失败 消息体中只维护发送成功与发送失败
	--必须,文件中已发送的设置为发送成功,文件中未发送的设置为发送失败,其它消息设置为发送中
	6:下载状态--下载中,已下载,未下载
	--本地用户发送的消息默认已下载
	--发送过来的外部文件消息默认未下载
	--系统与时间没有文件消息
	--界面只显示下载中,已下载 消息体中只维护已下载与未下载
	--只有文件消息才有下载状态,文件中已下载的设置为已下载,文件中未下载的设置为未下载,其它消息设置为下载中
	7:下载进度--下载进度条--界面只显示下载进度条 消息体中不维护下载进度条--只有文件消息才有下载进度
	*/
	QStandardItemModel *m_messageListModel;				 // 聊天消息数据模型
	QQChatMessageDelegate *m_messageListDelegate;		 // 聊天消息的样式代理
	ElaMenu *m_chatFriendContextMenu;					 // 聊天索引列表的右键菜单--数据源是friend
	ElaMenu *m_chatGroupContextMenu;					 // 聊天索引列表的右键菜单--数据源是group
	QModelIndex m_activeChatIndex;						 // 当前活动的聊天索引--主要是为了控制消息的显示
	QMap<std::string, QStandardItem *> m_haveInChatList; // 已经在聊天索引列表中的用户--主要是为了与友人界面的发送消息按钮进行关联
protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watch, QEvent *event) override;

private:
	/**
	 * @brief 请求网络模块进行简单的非实时消息发送
	 * @param message
	 */
	void sendMessageToNetWork(const QSharedPointer<QQConfigs::MessageConfig> &message);
	/**
	 * @brief 加载用户信息到聊天索引列表项中
	 * @param item
	 * @param user
	 */
	void loadPeopleInModelItem(QStandardItem *item, QQConfigs::FriendConfig *user);
	/**
	 * @brief 加载群组信息到聊天索引列表项中
	 * @param item
	 * @param group
	 */
	void loadPeopleInModelItem(QStandardItem *item, QQConfigs::GroupConfig *group);
	/**
	 * @brief 加载消息信息到聊天消息列表项中
	 * @param item
	 * @param message
	 */
	void loadMessageInModelItem(QStandardItem *item, const QSharedPointer<QQConfigs::MessageConfig> &message);
	/**
	 * @brief 通过文件名发送文件
	 * @param fileName
	 * @note 本质不发送文件，而是先更新界面，然后发送文件名到网络模块，网络模块发送文件
	 */
	void sendFileByName(const QString &fileName);
	/**
	 * @brief 向聊天消息列表中添加消息，主要是更新界面
	 * @param message
	 * @param index
	 * @note 本质不发送消息，而是先更新界面，然后发送消息到网络模块，网络模块发送消息，其中由index区分是新消息还是历史消息
	 * @note 如果是自己发送的消息,则发送给服务器--其余消息均重文件中加载(群聊与好友消息来源于服务器,下载到本地,系统消息与时间消息是由本地生成,存储于本地,不需要发送)
	 */
	void appendMessage(const QSharedPointer<QQConfigs::MessageConfig> &message, int index = -1);
	void loadMessages(QQConfigs::FriendConfig *user);
	void loadMessages(QQConfigs::GroupConfig *group);
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
};

#endif // QQ_CHAT_CHAT_H