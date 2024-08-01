#pragma once
// 聊天界面单个消息封装
#include <qwidget.h>
#include <qlabel.h>
#include <qicon.h>
#include <qevent.h>
#include <qstyleditemdelegate.h>
#include <qpainter.h>
#include <qmovie.h>
#include <qtimer.h>
#include <qpainterpath.h>
#include <qbuttongroup.h>
#include <qstandarditemmodel.h>
#include <qpropertyanimation.h>
#include <qsequentialanimationgroup.h>
#include <qscrollbar.h>
#include <qscrollarea.h>
#include <qmimedatabase.h>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <qfileinfo.h>
#include <qdesktopservices.h>
#include <QtConcurrent/qtconcurrentrun.h>

#include "qfunction.hpp"

class QQChatMessageSingleView : public QWidget
{
	Q_OBJECT
public:
	explicit QQChatMessageSingleView(QWidget *parent = nullptr);
	virtual ~QQChatMessageSingleView();
	void setMMessage(ClientConfigs::MessageBaseConfig *message);
	const ClientConfigs::MessageBaseConfig *getMMessage();
	void setMIsSending(bool isSending);

private:
	void setMFileIcon(const QString &mimeType);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	ClientConfigs::MessageBaseConfig *m_message;
	const qreal m_iconMargin = 25;
	const qreal m_contentMargin = 75;
	const qreal m_contentPadding = 10;
	const qreal m_nameMargin = 5;
	bool m_isHeightChanged;
	bool m_isSending;
	std::optional<bool> m_isDownLoading;	 // 是否正在下载--仅文件消息有效
	std::optional<qreal> m_downLoadProgress; // 下载进度--仅文件消息有效
	QRect m_frame;
	QRect m_iconRect;
	QRect m_nameRect;
	QRect m_stateRect;
	QPixmap m_stateIcon;
	QMovie *m_movie;

	QPixmap m_icon;
	QString m_name;
	std::optional<QString> m_textContent;
	std::optional<QPixmap> m_imageContent;

	std::optional<QString> m_fileMimeType;			  // 文件类型--仅文件消息有效
	std::optional<QPixmap> m_fileIcon;				  // 文件图标--仅文件消息有效
	std::optional<QFutureWatcher<QString>> m_watcher; // 异步获取文件类型--仅文件消息有效
signals:
	void sign_heightChanged(qreal height);
private slots:
	void do_futureGetFileMimeType();
	void do_stateIconMovieChanged();
};