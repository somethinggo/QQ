#pragma once
// 消息对话框
#include <qdialog.h>
#include <qlabel.h>
#include <qdialogbuttonbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qtimer.h>
#include <functional>

#include "qfunction.hpp"
#include "QQAppBar.h"

class QQMessageDialogButtonStyle : public QProxyStyle
{
public:
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const override;
};

class QQMessageDialog : public QDialog
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQMessageDialog)
private:
	explicit QQMessageDialog(QWidget *parent = nullptr);
	virtual ~QQMessageDialog();

public:
	void setMShadowWidget(QWidget *parent);				  // 设置阴影窗口--父窗口
	void setMTittle(const QString &tittle);				  // 设置标题
	void openOkButton();								  // 打开确定按钮
	void closeOkButton();								  // 关闭确定按钮
	void addMWidget(QWidget *widget);					  // 添加控件
	void removeAllMWidget();							  // 移除所有控件
	void setMAlertLabel(const QString &text = QString()); // 设置警告信息
	void setMFunction(std::function<void()> &function);	  // 设置确定按钮执行函数

private:
	void removeNextAllWidget(QLayout *layout);

protected:
	void showEvent(QShowEvent *event) override;
	void hideEvent(QHideEvent *event) override;

private:
	QQMessageDialogButtonStyle *m_proxyStyle;
	QWidget *m_shadowWidget;
	QQAppBar *m_appBar;
	QString m_tittle;
	QLabel *m_tittleLabel;
	QDialogButtonBox *m_buttonBox;
	std::optional<bool> m_isAlerting;
	std::optional<QString> m_alert;
	std::optional<std::function<void()>> m_function;
private slots:
	void do_accepted();
};
