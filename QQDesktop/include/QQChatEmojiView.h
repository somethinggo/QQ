#pragma once

#include <qdialog.h>
#include <qstackedwidget.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qtableview.h>
#include <qaction.h>
#include <qstandarditemmodel.h>
#include <qmovie.h>
#include <qtableview.h>
#include <qheaderview.h>
#include <qfiledialog.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <ElaMessageBar.h>

#include "qfunction.hpp"
#include "QQAppBar.h"
#include "QQActionTabBar.h"

class QQChatEmojiViewProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};
// emoji label--Unicode的emoji表情，用户的图片，gif动画
class QMEmojiLabel : public QLabel
{
	Q_OBJECT
public:
	QMEmojiLabel(QWidget *parent = nullptr);
	virtual ~QMEmojiLabel();
	void setMMovie(QMovie *movie);

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void enterEvent(QEnterEvent *event) override;
#else
	void enterEvent(QEvent *event) override;
#endif
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	bool m_isHovered;
	bool m_isPressed;
};

// 简便单个添加使用
class QMStandardItemModel : public QStandardItemModel
{
public:
	QMStandardItemModel(QObject *parent = nullptr);
	virtual ~QMStandardItemModel();
	void setStartPos(uint row = 0, uint column = 0);
	void insertData(QStandardItem *item);

private:
	int m_currentRow;
	int m_currentColumn;
};

class QQChatEmojiView : public QDialog
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQChatEmojiView)
private:
	explicit QQChatEmojiView(QWidget *parent = nullptr);
	virtual ~QQChatEmojiView();
	void appendTabView();											   // 添加新的表情组
	void appendEmojiByUnicode(char32_t *start, int len, char32_t end); // 添加Unicode表情
	void initEmojiView();											   // 初始化表情视图

private:
	const uint MAX_COLUMN_COUNT = 10;						  // 最大列数
	QString configFileName = "../userFile/emoji/config.json"; // 配置文件
	QQChatEmojiViewProxyStyle *m_proxyStyle;
	QQAppBar *m_appBar;
	QStackedWidget *m_stackedWidget;
	QQActionTabBar *m_tabBar;
	QList<QMStandardItemModel *> m_models;
signals:
	void sign_emojiItemClicked(const QString &emoji, ClientEnums::EmojiType type);
private slots:
	void do_emojiAppendBtnClicked();
	void do_emojiEmojiItemClicked(const QModelIndex &index);
};
