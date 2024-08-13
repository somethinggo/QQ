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
#include "QQChatIndexDelegate.h"
#include "QQChatMessageDelegate.h"
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

class QMSearchProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQChat : public QWidget
{
	Q_OBJECT

public:
	explicit QQChat(QWidget *parent = nullptr);
	virtual ~QQChat();
	void connectInit();
	void loadingInformation();

private:
	Ui::QQChatClass *ui;
	QQChatProxyStyle *m_commonStyle;
	QMSearchProxyStyle *m_searchStyle;
	QAction *m_searchAction;
	ElaMenu *m_searchMoreMenu;
	QButtonGroup *m_buttonGroup;

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
	QStandardItemModel *m_chatListModel;	 // 聊天索引列表数据模型
	QQChatIndexDelegate *m_chatListDelegate; // 聊天索引列表信息的样式代理
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
	QStandardItemModel *m_messageModel;					 // 聊天消息数据模型
	QQChatMessageDelegate *m_chatMessageDelegate;		 // 聊天消息的样式代理
	ElaMenu *m_chatFriendContextMenu;					 // 聊天索引列表的右键菜单--数据源是friend
	ElaMenu *m_chatGroupContextMenu;					 // 聊天索引列表的右键菜单--数据源是group
	QModelIndex m_activeChatIndex;						 // 当前活动的聊天索引--主要是为了控制消息的显示
	QMap<std::string, QStandardItem *> m_haveInChatList; // 已经在聊天索引列表中的用户--主要是为了与友人界面的发送消息按钮进行关联
protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watch, QEvent *event) override;

private:																															 // 接收消息--主要是接收后台的消息,并且设置模型中的项的状态信息
	void loadPeopleInModelItem(QStandardItem *item, QQConfigs::UserFriend_C *user);													 // 设置模型中的项的状态信息--主要是好友信息--好友体指针,好友索引窗口类型
	void loadPeopleInModelItem(QStandardItem *item, QQConfigs::UserGroup_C *group);													 // 设置模型中的项的状态信息--主要是群组信息--群组体指针,群组索引窗口类型
	void loadMessageInModelItem(QStandardItem *item, QSharedPointer<QQConfigs::MessageBaseConfig> &message, bool isReceive = false); // 设置模型中的项的状态信息--主要是消息信息--消息体指针,消息是否为接收的消息
	void sendFileByName(QString &fileName);
	void appendMessage(QSharedPointer<QQConfigs::MessageBaseConfig> &message, bool isUpdateData = true, int index = -1, bool isReceive = false); // 界面添加消息,选择更新数据--主要针对服务器的消息(向服务器发送的消息与接受来自服务器的消息),选择更新项索引,是否为接收的消息
	void loadMessages(QQConfigs::UserFriend_C *user);																							 // 界面加载消息组,主要针对文件中的消息--主要是加载好友的消息
	void loadMessages(QQConfigs::UserGroup_C *group);																							 // 界面加载消息组,主要针对文件中的消息--主要是加载群组的消息
private slots:
	void do_userClickSearchMoreButton();						// 用户点击搜索更多按钮
	void do_userClickSearchMoreButtonAction(QAction *action);	// 用户点击搜索更多按钮选项
	void do_userClickRightContextMenuAction(QAction *action);	// 用户点击聊天索引窗口右键菜单
	void do_userClickChatIndex(const QModelIndex &index);		// 用户点击聊天索引窗口索引
	void do_userClickSendEmojiButton();							// 用户点击发送表情按钮
	void do_userClickSendEmojiButtonFile(const QString &emoji); // 用户点击具体发送表情
	void do_userClickSendFileButton();							// 用户点击发送文件按钮
	void do_userClickSendPictureButton();						// 用户点击发送图片按钮
	void do_userOpenBigWriteButton(bool isChecked);				// 用户点击打开大写锁定
	void do_userClickSetFontButton();							// 用户点击设置字体按钮
	void do_userClickLookHistroyButton();						// 用户点击查看历史消息按钮
	void do_limitUserInputTextCount();							// 限制用户文本输入的数量
	void do_userClickSendAudioButton();							// 用户点击发送语音按钮
	void do_userClickEscInAudioView();							// 用户点击退出语音界面
	void do_userAudioDataOverTheLimit(int time);				// 用户输入的语音时长超限										  // 用户语音数据超过限制
	void do_userClickClearInputButton();						// 用户点击清空输入按钮
	void do_userClickSendMessageButton();						// 用户点击发送消息按钮
};

#endif // QQ_CHAT_CHAT_H