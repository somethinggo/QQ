#pragma once

#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpropertyanimation.h>
#include <qscreen.h>
#include <qapplication.h>
#include <qabstractnativeeventfilter.h>
#include <optional>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#endif

#include <ElaIcon.h>
#include <ElaIconButton.h>

#include "qfunction.hpp"

class QQAppBar : public QWidget, QAbstractNativeEventFilter
{
	Q_OBJECT
	Q_MPROPERTY_CREATE(bool, MIsAllowDrag, m_isAllowDrag)
	Q_MPROPERTY_CREATE(bool, MIsAllowResize, m_isAllowResize)
public:
	explicit QQAppBar(QWidget *parent = nullptr, ClientEnums::AppBarHintType types = ClientEnums::AppBarHintType::WindowsHint);
	virtual ~QQAppBar();
	void setMWindowsFlag(ClientEnums::AppBarHintType type, bool isEnable = true);
	void setMWindowsFlags(ClientEnums::AppBarHintTypes types);
	ClientEnums::AppBarHintTypes getMWindowsFlags();
	void setMWindowsTitle(const QString &title, Qt::AlignmentFlag flag = Qt::AlignmentFlag::AlignCenter);
	void setMWindowsIcon(const QIcon &icon);

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
#else
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#endif

private:
	WId m_wid;
	ClientEnums::AppBarHintTypes m_types;
	int m_margin;
	QIcon m_icon;
	QLabel *m_iconLabel;
	QString m_title;
	QLabel *m_titleLabel;
	std::optional<ElaIconButton *> m_minimizeButton;
	std::optional<ElaIconButton *> m_closeButton;
	std::optional<ElaIconButton *> m_maximizeButton;
};
