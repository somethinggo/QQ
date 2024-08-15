#ifndef QQ_EMOJI_EMOJI_H
#define QQ_EMOJI_EMOJI_H

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

#include "qqglobal.h"
#include "qqwidget.h"
#include "QQEmojiLab.h"

class QQEmojiProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQEmoji : public QDialog
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQEmoji)
private:
	explicit QQEmoji(QWidget *parent = nullptr);
	virtual ~QQEmoji();
	void appendTabView();											   // 添加新的表情组
	void appendEmojiByUnicode(char32_t *start, int len, char32_t end); // 添加Unicode表情
	void insertData(int index, QStandardItem *item);				   // 更新位置
	void initEmojiView();											   // 初始化表情视图
	void loadEmojiData(const QByteArray &data);						   // 加载表情数据
protected:
	bool event(QEvent *event) override;

private:
	const uint MAX_COLUMN_COUNT = 10; // 最大列数
	QProxyStyle *m_proxyStyle;
	QQWidgets::AppBar *m_appBar;
	QStackedWidget *m_stackedWidget;
	QQWidgets::AnimationTabBar *m_tabBar;
	QList<QPair<QStandardItemModel *, QPoint>> m_models; // 当前表情组--表情模型,最后一个表情位置
private slots:
	void do_emojiAppendBtnClicked();
	void do_emojiEmojiItemClicked(const QModelIndex &index);
};

#endif // QQ_EMOJI_EMOJI_H
