#pragma once

#include <qwidget.h>
#include <qudpsocket.h>
#include <qtextedit.h>
#include <qevent.h>
#include <qtimer.h>
#include <qbuttongroup.h>
#include <qstandarditemmodel.h>
#include <qfiledialog.h>
#include <qdialogbuttonbox.h>
#include <qtabbar.h>
#include <qwidgetaction.h>
#include <qtextobject.h>
#include <qcombobox.h>
#include <qbuffer.h>
#include <qfileiconprovider.h>
#include <qgraphicseffect.h>

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

#include <ElaComboBox.h>
#include <ElaLineEdit.h>
#include <ElaMenu.h>
#include <ElaFlowLayout.h>
#include <ElaMessageBar.h>
#include <ElaIcon.h>

#include "QQMessageDialog.h"
#include "QQActionTabBar.h"
#include "QQSinglePeopleWidget.h"
#include "QQChatEmojiView.h"
#include "QQChatMessageSingleView.h"
#include "QQCustomDelegate.h"
#include "QQInformationListWidget.h"
#include "QQAppendFriendOrGroupDialog.h"
#include "QQSearchHistoryLabel.h"
#include "QMCircleSingleView.h"
#include "QMSkinDesSingleView.h"
#include "ui_QQDesktop.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QQDesktopClass;
};
QT_END_NAMESPACE

class QMCommonProxyStyle : public QProxyStyle
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

class QQDesktop : public QWidget
{
	Q_OBJECT

public:
	QQDesktop(QWidget *parent = nullptr);
	virtual ~QQDesktop();
	void connectInit();
	void do_loadingInformation(); // 初始化加载文件信息--主要是加载文件信息

private:
	Ui::QQDesktopClass *ui;
	QList<ClientConfigs::UserFriend_C *> m_friends; // 好友列表
	QMCommonProxyStyle *m_commonStyle;				// 通用样式代理--使用name(唯一名)区别控件
	QMSearchProxyStyle *m_searchStyle;
	QQAppBar *m_appBar; // 顶部工具栏
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
	QStandardItemModel *m_chatListModel;			  // 聊天索引列表数据模型
	QQSinglePeopleWidgetDelegate *m_chatListDelegate; // 聊天索引列表信息的样式代理
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
	QStandardItemModel *m_messageListModel;					// 聊天消息数据模型
	QQChatMessageSingleViewDelegate *m_chatMessageDelegate; // 聊天消息的样式代理
	ElaMenu *m_chatFriendContextMenu;						// 聊天索引列表的右键菜单--数据源是friend
	ElaMenu *m_chatGroupContextMenu;						// 聊天索引列表的右键菜单--数据源是group
	QModelIndex m_activeChatIndex;							// 当前活动的聊天索引--主要是为了控制消息的显示
	QMap<std::string, QStandardItem *> m_haveInChatList;	// 已经在聊天索引列表中的用户--主要是为了与友人界面的发送消息按钮进行关联

	QStandardItemModel *m_friendsListModel;					// 好友列表数据模型
	QStandardItemModel *m_groupsListModel;					// 群组列表数据模型
	QQSingleFriendWidgetDelegate *m_friendAndGroupDelegate; // 好友列表与群组列表的样式代理
	ElaMenu *m_friendContextMenu;							// 好友列表的右键菜单--数据源是friend--群组默认缺失
	QQActionTabBar *m_friendAndGroupTabBar;					// 友人界面的好友群组选项卡
	QMap<QString, QModelIndex> m_friendsGroups;				// 用户对好友的分组--配置文件中加载--异步操作配置文件
	ElaComboBox *m_friendsGroupComboBox;					// 好友分组选择器--主要是用于好友的分组选择
protected:
	bool eventFilter(QObject *watch, QEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	void do_sendmsg(QStandardItem *item, QSharedPointer<ClientConfigs::MessageBaseConfig> &message);																	// 设置模型中的项的状态信息--主要是发送成功与否状态,并且向后台发送消息
	void do_revmsg();																																					// 接收消息--主要是接收后台的消息,并且设置模型中的项的状态信息
	void do_peopleInModelItem(QStandardItem *item, ClientConfigs::UserFriend_C *user, ClientEnums::PeopleWidgetType type = ClientEnums::PeopleWidgetType::friendchats); // 设置模型中的项的状态信息--主要是好友信息--好友体指针,好友索引窗口类型
	void do_peopleInModelItem(QStandardItem *item, ClientConfigs::UserGroup_C *group, ClientEnums::PeopleWidgetType type = ClientEnums::PeopleWidgetType::groupchats);	// 设置模型中的项的状态信息--主要是群组信息--群组体指针,群组索引窗口类型
	void do_messageInModelItem(QStandardItem *item, QSharedPointer<ClientConfigs::MessageBaseConfig> &message, bool isReceive = false);									// 设置模型中的项的状态信息--主要是消息信息--消息体指针,消息是否为接收的消息
	void do_sendFileByName(QString &fileName);
	void do_appendMessage(QSharedPointer<ClientConfigs::MessageBaseConfig> &message, bool isUpdateData = true, int index = -1, bool isReceive = false); // 界面添加消息,选择更新数据--主要针对服务器的消息(向服务器发送的消息与接受来自服务器的消息),选择更新项索引,是否为接收的消息
	void do_loadMessages(ClientConfigs::UserFriend_C *user);																							// 界面加载消息组,主要针对文件中的消息--主要是加载好友的消息
	void do_loadMessages(ClientConfigs::UserGroup_C *group);																							// 界面加载消息组,主要针对文件中的消息--主要是加载群组的消息
private slots:
	void do_userClickSearchMoreButton();														  // 用户点击搜索更多按钮
	void do_userClickSearchMoreButtonAction(QAction *action);									  // 用户点击搜索更多按钮选项
	void do_chat_userClickRightContextMenuAction(QAction *action);								  // 用户点击聊天索引窗口右键菜单
	void do_chat_userClickChatIndex(const QModelIndex &index);									  // 用户点击聊天索引窗口索引
	void do_chat_userClickSendEmojiButton();													  // 用户点击发送表情按钮
	void do_chat_userClickSendEmojiButtonFile(const QString &emoji, ClientEnums::EmojiType type); // 用户点击具体发送表情
	void do_chat_userClickSendFileButton();														  // 用户点击发送文件按钮
	void do_chat_userClickSendPictureButton();													  // 用户点击发送图片按钮
	void do_chat_userOpenBigWriteButton(bool isChecked);										  // 用户点击打开大写锁定
	void do_chat_userClickSetFontButton();														  // 用户点击设置字体按钮
	void do_chat_userClickLookHistroyButton();													  // 用户点击查看历史消息按钮
	void do_chat_limitUserInputTextCount();														  // 限制用户文本输入的数量
	void do_chat_userClickSendAudioButton();													  // 用户点击发送语音按钮
	void do_chat_userClickEscInAudioView();														  // 用户点击退出语音界面
	void do_chat_userAudioDataOverTheLimit(int time);											  // 用户输入的语音时长超限										  // 用户语音数据超过限制
	void do_chat_userClickClearInputButton();													  // 用户点击清空输入按钮
	void do_chat_userClickSendMessageButton();													  // 用户点击发送消息按钮
	void do_friend_userClickRightContextMenu(const QPoint &pos);								  // 用户点击好友列表点击右键菜单--首个不触发
	void do_friend_userClickRightContextMenuAction(QAction *action);							  // 用户点击好友列表右键菜单选项
	void do_friend_userClickNoticeButton();														  // 用户点击通知按钮--好友通知与群通知
	void do_friend_userClickOption(int index);													  // 用户点击好友界面的选项卡
	void do_friend_userClickFriendListIndex(const QModelIndex &index);							  // 用户点击好友界面的好友列表--打开分组列表,显示好友信息
	void do_friend_userClickGroupListIndex(const QModelIndex &index);							  // 用户点击好友界面的群组列表--打开分组列表,显示群信息
	void do_friend_userSelectedGroupChanged(const QString &text);								  // 用户选择好友分组--主要是用于好友的分组更改
	void do_friend_userClickShareOtherButton();													  // 用户点击分享给其他按钮
	void do_friend_userClickMusicConnectButton();												  // 用户点击音乐连接按钮
	void do_friend_userClickSendMessageButton();												  // 用户点击发送消息按钮
};
