#include "QQAppBar.h"

QQAppBar::QQAppBar(QWidget *parent, ClientEnums::AppBarHintType types)
	: QWidget(parent), m_types(types)
{
	this->window()->setAttribute(Qt::WA_Mapped);
	qApp->installNativeEventFilter(this);
	this->setFixedHeight(30);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::transparent);
	this->setPalette(palette);
	this->setAutoFillBackground(true);

	m_wid = this->window()->winId();
	m_margin = 5;
	m_isAllowDrag = true;
	m_isAllowResize = true;
	m_iconLabel = new QLabel(this);
	m_iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_titleLabel = new QLabel(this);
	m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_iconLabel);
	layout->addWidget(m_titleLabel);
	this->setLayout(layout);
	setMWindowsFlags(m_types);

	HWND hwnd = reinterpret_cast<HWND>(this->window()->winId());
	DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
	::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
	SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

QQAppBar::~QQAppBar()
{
}

ClientEnums::AppBarHintTypes QQAppBar::getMWindowsFlags()
{
	return m_types;
}

void QQAppBar::setMWindowsFlag(ClientEnums::AppBarHintType type, bool isEnable)
{
	if (isEnable)
	{
		setMWindowsFlags(m_types | type);
	}
	else
	{
		int temp = static_cast<int>(type);
		setMWindowsFlags(m_types & ~temp);
	}
}

void QQAppBar::setMWindowsFlags(ClientEnums::AppBarHintTypes types)
{
	m_types = types;
	if (m_types.testFlag(ClientEnums::AppBarHintType::NoButtonHint))
	{
		this->setFixedHeight(0);
		return;
	}
	if (m_types.testFlag(ClientEnums::AppBarHintType::MinimizeButtonHint))
	{
		m_minimizeButton.emplace(new ElaIconButton(ElaIconType::Dash, 12, 40, 30, this));
		m_minimizeButton.value()->setLightHoverColor(QColor(0xEF, 0xE6, 0xED));
		connect(m_minimizeButton.value(), &ElaIconButton::clicked, [this]
				{ this->window()->showMinimized(); });
		qobject_cast<QHBoxLayout *>(this->layout())->addWidget(m_minimizeButton.value());
	}
	if (m_types.testFlag(ClientEnums::AppBarHintType::MaximizeButtonHint))
	{
		m_maximizeButton.emplace(new ElaIconButton(ElaIconType::Square, 13, 40, 30, this));
		m_maximizeButton.value()->setLightHoverColor(QColor(0xEF, 0xE6, 0xED));
		connect(m_maximizeButton.value(), &ElaIconButton::clicked, [this]
				{
				if (this->window()->isMaximized())
				{
					this->window()->showNormal();
					m_maximizeButton.value()->setAwesome(ElaIconType::Square);
				}
				else
				{
					this->window()->showMaximized();
					m_maximizeButton.value()->setAwesome(ElaIconType::WindowRestore);
				} });
		qobject_cast<QHBoxLayout *>(this->layout())->addWidget(m_maximizeButton.value());
	}
	if (m_types.testFlag(ClientEnums::AppBarHintType::CloseButtonHint))
	{
		m_closeButton.emplace(new ElaIconButton(ElaIconType::Xmark, 17, 40, 30, this));
		m_closeButton.value()->setLightHoverColor(QColor(0xC4, 0x2B, 0x1C));
		m_closeButton.value()->setDarkHoverColor(QColor(0xC4, 0x2B, 0x1C));
		connect(m_closeButton.value(), &ElaIconButton::clicked, [this]
				{
				QRect windowRect = window()->geometry();
				this->window()->setMinimumSize(0, 0);
				this->window()->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
				QPropertyAnimation* closeGeometryAnimation = new QPropertyAnimation(this->window(), "geometry");
				QPropertyAnimation* closeOpacityAnimation = new QPropertyAnimation(this->window(), "windowOpacity");
				connect(closeGeometryAnimation, &QPropertyAnimation::finished, this, [=]()
					{
						this->window()->close();
					});
				closeGeometryAnimation->setStartValue(windowRect);
				qreal minmumWidth = (m_titleLabel->width() + 320);
				closeGeometryAnimation->setEndValue(QRect(windowRect.x() + (windowRect.width() - minmumWidth) / 2, windowRect.y() + (windowRect.height() / 2) - 145, minmumWidth, 290));
				closeGeometryAnimation->setDuration(300);
				closeGeometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
				closeOpacityAnimation->setStartValue(1);
				closeOpacityAnimation->setEndValue(0);
				closeOpacityAnimation->setDuration(300);
				closeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
				closeGeometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				closeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				this->close(); });
		qobject_cast<QHBoxLayout *>(this->layout())->addWidget(m_closeButton.value());
	}
}

void QQAppBar::setMWindowsTitle(const QString &title, Qt::AlignmentFlag flag)
{
	m_title = title;
	m_titleLabel->setText(m_title);
	m_titleLabel->setAlignment(flag);
}

void QQAppBar::setMWindowsIcon(const QIcon &icon)
{
	m_icon = icon;
	m_iconLabel->setPixmap(m_icon.pixmap(20, 20));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool QQAppBar::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
#else
bool QQAppBar::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
#endif
{
#ifdef Q_OS_WIN

	if (message == nullptr)
	{
		return false;
	}
	const MSG *msg = static_cast<MSG *>(message);
	const HWND hwnd = msg->hwnd;
	if (msg == nullptr || hwnd == nullptr)
	{
		return false;
	}

	//  只处理标题栏顶层窗口的事件
	if (reinterpret_cast<qint64>(hwnd) != m_wid)
	{
		return false;
	}
	const WPARAM wParam = msg->wParam;
	const LPARAM lParam = msg->lParam;

	switch (msg->message)
	{
	// 优化窗口大小时的闪烁问题,减少重绘次数
	case WM_WINDOWPOSCHANGING:
	{
		WINDOWPOS *wp = reinterpret_cast<WINDOWPOS *>(wParam);
		if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
		{
			wp->flags |= SWP_NOCOPYBITS;
			*result = ::DefWindowProcW(hwnd, msg->message, wParam, lParam);
			return true;
		}
		return false;
	}
	// 去除默认的标题栏
	case WM_NCCALCSIZE:
	{
		if (wParam == FALSE)
		{
			return false;
		}
		RECT *clientRect = &((NCCALCSIZE_PARAMS *)(lParam))->rgrc[0];
		if (!::IsZoomed(hwnd))
		{
			clientRect->top -= 1;
			clientRect->bottom -= 1;
		}
		else
		{
			const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam);
			if ((hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE))
			{
				*result = static_cast<long>(hitTestResult);
				return true;
			}
			auto geometry = window()->screen()->geometry();
			clientRect->top = geometry.top();
		}
		*result = WVR_REDRAW;
		return true;
	}
	// 计算鼠标位置,更新状态信息
	case WM_NCHITTEST:
	{
		// 获取鼠标位置,转换为客户区坐标,计算边界用于大小调整
		if (m_isAllowResize)
		{
			POINT nativeLocalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
			::ScreenToClient(hwnd, &nativeLocalPos);
			RECT clientRect{0, 0, 0, 0};
			::GetClientRect(hwnd, &clientRect);
			auto clientWidth = clientRect.right - clientRect.left;
			auto clientHeight = clientRect.bottom - clientRect.top;
			bool left = nativeLocalPos.x < m_margin;
			bool right = nativeLocalPos.x > clientWidth - m_margin;
			bool top = nativeLocalPos.y < m_margin;
			bool bottom = nativeLocalPos.y > clientHeight - m_margin;
			*result = 0;
			if (!this->window()->isFullScreen() && !this->window()->isMaximized())
			{
				if (left && top)
				{
					*result = HTTOPLEFT;
				}
				else if (left && bottom)
				{
					*result = HTBOTTOMLEFT;
				}
				else if (right && top)
				{
					*result = HTTOPRIGHT;
				}
				else if (right && bottom)
				{
					*result = HTBOTTOMRIGHT;
				}
				else if (left)
				{
					*result = HTLEFT;
				}
				else if (right)
				{
					*result = HTRIGHT;
				}
				else if (top)
				{
					*result = HTTOP;
				}
				else if (bottom)
				{
					*result = HTBOTTOM;
				}
			}
			if (0 != *result)
			{
				return true;
			}
		}
		// 当前鼠标位置在自定义标题栏内
		if (getMouseIsInWidget(m_titleLabel) || getMouseIsInWidget(m_iconLabel))
		{
			*result = HTCAPTION;
			return true;
		}
		return QWidget::nativeEvent(eventType, message, result);
	}
	// 鼠标左键按下时,移动窗口
	case WM_LBUTTONDOWN:
	{
		if (m_isAllowDrag && (getMouseIsInWidget(m_titleLabel) || getMouseIsInWidget(m_iconLabel)))
		{
			ReleaseCapture();
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		return QWidget::nativeEvent(eventType, message, result);
	}
	}
#endif
	return QWidget::nativeEvent(eventType, message, result);
}