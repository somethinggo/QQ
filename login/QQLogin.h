#ifndef QQ_LOGIN_LOGIN_H
#define QQ_LOGIN_LOGIN_H

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

#include "ui_QQLogin.h"

#include "common/qqfunction.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class QQLoginClass;
};
QT_END_NAMESPACE

class QQLoginProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQLogin : public QWidget
{
	Q_OBJECT
	QQ_SINGLETON_CREATE(QQLogin)
public:
	explicit QQLogin(QWidget *parent = nullptr);
	~QQLogin();

private:
	void disEnableAllWidget(bool isEnable);
	void login(const QByteArray &data);
	void registered(const QByteArray &data);
	void findPassword(const QByteArray &data);

protected:
	bool event(QEvent *event) override;
	bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
	void do_senderEnableBtn();
	void do_changedCurrentIndex();
	void do_loginBtnClicked();
	void do_registerBtnClicked();
	void do_findPasswordBtnClicked();

private:
	Ui::QQLoginClass *ui;
	QValidator *m_accountValidator;
	QValidator *m_numberValidator;
	QValidator *m_passwordValidator;
	QProxyStyle *m_proxyStyle;
	QtMaterialCircularProgress *m_progress;
	uint64_t m_count;
};

#endif // QQ_LOGIN_LOGIN_H