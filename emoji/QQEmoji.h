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

#include "common/qqwidget.h"

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
	QQ_SINGLETON_CREATE(QQEmoji)
public:
	explicit QQEmoji(QWidget *parent = nullptr);
	~QQEmoji();

private:
	void appendTabView();
	void appendEmojiByUnicode(char32_t *start, int len, char32_t end);
	void insertData(int index, QStandardItem *item);
	void initEmojiView();

	void handleStorage(const QByteArray &data);

protected:
	bool event(QEvent *event) override;

private slots:
	void do_emojiAppendBtnClicked();
	void do_emojiEmojiItemClicked(const QModelIndex &index);

private:
	const uint MAX_COLUMN_COUNT = 10;
	QProxyStyle *m_proxyStyle;
	QQWidgets::AppBar *m_appBar;
	QStackedWidget *m_stackedWidget;
	QQWidgets::AnimationTabBar *m_tabBar;
	QList<QPair<QStandardItemModel *, QPoint>> m_models; // 当前表情组--表情模型,最后一个表情位置
};

#endif // QQ_EMOJI_EMOJI_H
