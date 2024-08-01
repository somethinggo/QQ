#pragma once
#include <qradiobutton.h>
#include <qpainterpath.h>
#include <qrect.h>
#include <qpainter.h>
#include <qevent.h>
#include <qtimer.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qbuttongroup.h>

class QQOptionFillButtonProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQOptionFillButton : public QRadioButton
{
	Q_OBJECT
public:
	explicit QQOptionFillButton(QWidget *parent = nullptr);
	virtual ~QQOptionFillButton();
	void setIcon(const QIcon &icon);

protected:
	void paintEvent(QPaintEvent *event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void enterEvent(QEnterEvent *event) override;
#else
	void enterEvent(QEvent *event) override;
#endif
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QQOptionFillButtonProxyStyle *m_proxyStyle;
	uint64_t m_newMsgCount;
	QRect m_newMsgRect;
	QTimer *m_timer;
	qint64 m_width;
	qint64 m_height;
	qint8 m_round;
	bool m_isIn;
	QIcon m_icon;
private slots:
	void do_timeout();
	void do_toggled(bool isChecked);
};
