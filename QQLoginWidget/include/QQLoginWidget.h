#pragma once

#include <qdialog.h>
#include <qproxystyle.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qevent.h>
#include <qpalette.h>
#include <qstyleoption.h>
#include <qvalidator.h>
#include <qlocalsocket.h>
#include <qmessagebox.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qtimer.h>
#include <qaction.h>
#include <qtmaterialcircularprogress.h>

#include <ElaMessageBar.h>

#include "qfunction.hpp"
#include "QMStackedWidget.h"
#include "ui_QQLoginWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QQLoginWidgetClass;
};
QT_END_NAMESPACE

class QMCommonProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQLoginWidget : public QWidget
{
	Q_OBJECT
public:
	QQLoginWidget(QWidget *parent = nullptr);
	~QQLoginWidget();
	void disEnableAllWidget(bool isEnable);

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
	void do_handleLocalSocketReadyRead();
	void do_senderEnableBtn();
	void do_changedCurrentIndex();
	void do_loginBtnClicked();
	void do_registerBtnClicked();
	void do_findPasswordBtnClicked();

private:
	Ui::QQLoginWidgetClass *ui;
	QValidator *m_accountValidator;			// 验证输入的是否为账号
	QValidator *m_numberValidator;			// 验证输入的是否为号码
	QValidator *m_passwordValidator;		// 验证输入的是否为密码
	QStyle *m_proxyStyle;					// 自定义样式
	QtMaterialCircularProgress *m_progress; // 点击登录按钮时的进度条
	uint64_t m_count;						// 记录点击登录按钮的次数
};
