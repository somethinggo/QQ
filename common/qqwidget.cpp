﻿#include "qqwidget.h"

namespace QQWidgets
{

    AppBar::AppBar(QWidget *parent, QQEnums::AppBarHintType types)
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
        setIsAllowDrag(true);
        setIsAllowResize(true);
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
        setWindowsFlags(m_types);

#ifdef Q_OS_WIN
        HWND hwnd = reinterpret_cast<HWND>(this->window()->winId());
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
#endif
    }

    AppBar::~AppBar()
    {
    }

    QQEnums::AppBarHintTypes AppBar::getWindowsFlags()
    {
        return m_types;
    }

    void AppBar::setWindowsFlag(QQEnums::AppBarHintType type, bool isEnable)
    {
        if (isEnable)
        {
            setWindowsFlags(m_types | type);
        }
        else
        {
            int temp = static_cast<int>(type);
            setWindowsFlags(m_types & ~temp);
        }
    }

    void AppBar::setWindowsFlags(QQEnums::AppBarHintTypes types)
    {
        m_types = types;
        if (m_types.testFlag(QQEnums::AppBarHintType::NoButtonHint))
        {
            this->setFixedHeight(0);
            return;
        }
        if (m_types.testFlag(QQEnums::AppBarHintType::MinimizeButtonHint))
        {
            m_minimizeButton.emplace(new ElaIconButton(ElaIconType::Dash, 12, 40, 30, this));
            m_minimizeButton.value()->setLightHoverColor(QColor(0xEF, 0xE6, 0xED));
            connect(m_minimizeButton.value(), &ElaIconButton::clicked, [this]
                    { this->window()->showMinimized(); });
            qobject_cast<QHBoxLayout *>(this->layout())->addWidget(m_minimizeButton.value());
        }
        if (m_types.testFlag(QQEnums::AppBarHintType::MaximizeButtonHint))
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
        if (m_types.testFlag(QQEnums::AppBarHintType::CloseButtonHint))
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

    void AppBar::setWindowsTitle(const QString &title, Qt::AlignmentFlag flag)
    {
        m_title = title;
        m_titleLabel->setText(m_title);
        m_titleLabel->setAlignment(flag);
    }

    void AppBar::setWindowsIcon(const QIcon &icon)
    {
        m_icon = icon;
        m_iconLabel->setPixmap(m_icon.pixmap(this->height(), this->height()));
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool AppBar::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
#else
    bool AppBar::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
#endif
    {
#ifdef Q_OS_WIN
        if ((eventType != "windows_generic_MSG") || !message)
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
            RECT *QQRect = &((NCCALCSIZE_PARAMS *)(lParam))->rgrc[0];
            if (!::IsZoomed(hwnd))
            {
                QQRect->top -= 1;
                QQRect->bottom -= 1;
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
                QQRect->top = geometry.top();
            }
            *result = WVR_REDRAW;
            return true;
        }
        // 计算鼠标位置,更新状态信息
        case WM_NCHITTEST:
        {
            // 获取鼠标位置,转换为客户区坐标,计算边界用于大小调整
            if (getIsAllowResize())
            {
                POINT nativeLocalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                ::ScreenToClient(hwnd, &nativeLocalPos);
                RECT QQRect{0, 0, 0, 0};
                ::GetClientRect(hwnd, &QQRect);
                auto QQWidth = QQRect.right - QQRect.left;
                auto QQHeight = QQRect.bottom - QQRect.top;
                bool left = nativeLocalPos.x < m_margin;
                bool right = nativeLocalPos.x > QQWidth - m_margin;
                bool top = nativeLocalPos.y < m_margin;
                bool bottom = nativeLocalPos.y > QQHeight - m_margin;
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
            if (QQFunctions::getMouseIsInWidget(m_titleLabel) || QQFunctions::getMouseIsInWidget(m_iconLabel))
            {
                *result = HTCAPTION;
                return true;
            }
            return QWidget::nativeEvent(eventType, message, result);
        }
        // 鼠标左键按下时,移动窗口
        case WM_LBUTTONDOWN:
        {
            if (getIsAllowDrag() && (QQFunctions::getMouseIsInWidget(m_titleLabel) || QQFunctions::getMouseIsInWidget(m_iconLabel)))
            {
                ReleaseCapture();
                SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }
            return QWidget::nativeEvent(eventType, message, result);
        }
        default:
            break;
        }

#else Q_OS_LINUX
        if (eventType != "xcb_generic_event_t" || message == nullptr)
        {
            return false;
        }
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);
        uint8_t responseType = event->response_type & 0x7f;
        switch (responseType)
        {
        case XCB_CONFIGURE_NOTIFY:
        {
            xcb_configure_notify_event_t *configureEvent = reinterpret_cast<xcb_configure_notify_event_t *>(event);
            if (configureEvent->window != wid)
            {
                return false;
            }
            if (configureEvent->width != width() || configureEvent->height != height())
            {
                return true;
            }
            break;
        }

        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t *motionEvent = reinterpret_cast<xcb_motion_notify_event_t *>(event);
            if (motionEvent->event != wid)
            {
                return false;
            }
            if (getIsAllowDrag() && (QQFunctions::getMouseIsInWidget(m_titleLabel) || QQFunctions::getMouseIsInWidget(m_iconLabel)))
            {
                XMoveWindow(QX11Info::display(), wid, motionEvent->root_x, motionEvent->root_y);
                return true;
            }
            break;
        }
        default:
            break;
        }
#endif
        return QWidget::nativeEvent(eventType, message, result);
    }

    void AnimationTabBar::QQProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == PE_FrameTabBarBase)
        {
            AnimationTabBar *tabBar = const_cast<AnimationTabBar *>(qobject_cast<const AnimationTabBar *>(widget));
            painter->save();
            QPainterPath path;
            path.addRoundedRect(widget->rect(), 5, 5);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->fillPath(path, QBrush(tabBar->getBackgroundColor()));
            painter->restore();
            return;
        }
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    void AnimationTabBar::QQProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTab)
        {
            QStyleOptionTab *tabOption = const_cast<QStyleOptionTab *>(qstyleoption_cast<const QStyleOptionTab *>(option));
            AnimationTabBar *tabBar = const_cast<AnimationTabBar *>(qobject_cast<const AnimationTabBar *>(widget));
            if (tabOption && tabBar)
            {
                QMargins margins = tabBar->getContentsMargins();
                int width = tabOption->rect.width();
                int height = tabOption->rect.height();
                tabOption->rect.setX(tabOption->rect.x() + margins.left());
                tabOption->rect.setY(tabOption->rect.y() + margins.top());
                tabOption->rect.setWidth(width - margins.left() - margins.right());
                tabOption->rect.setHeight(height - margins.top() - margins.bottom());
                if (!tabOption->icon.isNull())
                {
                    QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::Off);
                    QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
                    painter->drawPixmap(pos, pixmap);
                    painter->setPen(Qt::color1);
                    painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
                }
                else
                {
                    painter->setPen(Qt::color1);
                    painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
                }
            }
            if (tabOption && (tabOption->state & State_MouseOver))
            {
                // 绘制鼠标悬停,改变背景颜色,改变图标
                tabOption->state &= ~State_MouseOver;
                QPainterPath path;
                path.addRoundedRect(option->rect, 5, 5);
                painter->setRenderHint(QPainter::Antialiasing);
                painter->fillPath(path, QBrush(tabBar->getHoverColor()));
                if (!tabOption->icon.isNull())
                {
                    QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::On);
                    QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
                    painter->drawPixmap(pos, pixmap);
                    painter->setPen(tabBar->getSelectTextColor());
                    painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
                }
                else
                {
                    painter->setPen(tabBar->getSelectTextColor());
                    painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
                }
            }
            if (tabOption && (tabOption->state & State_Selected))
            {
                // 绘制选中,改变背景颜色,改变图标
                tabOption->state &= ~State_Selected;
                tabOption->state &= ~State_MouseOver;
                QPainterPath path;
                path.addRoundedRect(option->rect, 5, 5);
                painter->setRenderHint(QPainter::Antialiasing);
                painter->fillPath(path, QBrush(tabBar->getSelectColor()));
                if (!tabOption->icon.isNull())
                {
                    QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::On);
                    QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
                    painter->drawPixmap(pos, pixmap);
                    painter->setPen(tabBar->getSelectTextColor());
                    painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
                }
                else
                {
                    painter->setPen(tabBar->getSelectTextColor());
                    painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
                }
            }
            return;
        }
        return QProxyStyle::drawControl(element, option, painter, widget);
    }

    AnimationTabBar::AnimationTabBar(QWidget *parent)
        : QTabBar(parent)
    {
        setMaxWidth(0);
        setBackgroundColor(Qt::white);
        setHoverColor(Qt::black);
        setSelectColor(Qt::black);
        setSelectTextColor(Qt::white);
        m_proxyStyle = new QQProxyStyle;
        this->setStyle(m_proxyStyle);
    }

    AnimationTabBar::~AnimationTabBar()
    {
        delete m_proxyStyle;
    }

    void AnimationTabBar::setMargin(int margin)
    {
        m_margins = QMargins(margin, margin, margin, margin);
    }

    void AnimationTabBar::setContentsMargins(int left, int top, int right, int bottom)
    {
        m_margins = QMargins(left, top, right, bottom);
    }

    void AnimationTabBar::setContentsMargins(const QMargins &margins)
    {
        m_margins = margins;
    }

    QMargins AnimationTabBar::getContentsMargins() const
    {
        return m_margins;
    }

    QSize AnimationTabBar::tabSizeHint(int index) const
    {
        int width = getMaxWidth() == 0 ? this->width() / this->count() : getMaxWidth();
        int height = this->height();
        return QSize(width, height);
    }

    MessageDialog::MessageDialog(QWidget *parent)
        : QDialog(parent)
    {
        this->setFixedWidth(420);
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::white);
        this->setPalette(palette);
        m_tittle = QString();

        m_shadowWidget = new QWidget(this);
        QPalette shadowWidgetPalette = m_shadowWidget->palette();
        shadowWidgetPalette.setColor(QPalette::Window, QColor(0, 0, 0, 90));
        m_shadowWidget->setPalette(shadowWidgetPalette);
        m_shadowWidget->setAutoFillBackground(true);

        m_appBar = new AppBar(this, QQEnums::AppBarHintType::NoButtonHint);
        m_appBar->setIsAllowDrag(false);
        m_appBar->setIsAllowResize(false);
        m_tittleLabel = new QLabel(this);
        m_tittleLabel->setAlignment(Qt::AlignCenter);
        m_tittleLabel->setFont(QFont("Microsoft YaHei", 9));
        QPalette tittleLabelPalette = m_tittleLabel->palette();
        tittleLabelPalette.setColor(QPalette::WindowText, QColor("#7F7F7F"));
        m_tittleLabel->setPalette(tittleLabelPalette);
        m_tittleLabel->setFixedHeight(20);

        m_buttonBox = new QDialogButtonBox(this);
        m_buttonBox->setOrientation(Qt::Horizontal);
        QPushButton *okButton = m_buttonBox->addButton(QString::fromLocal8Bit("确定"), QDialogButtonBox::AcceptRole);
        okButton->setFixedSize(QSize(100, 40));
        okButton->setFont(QFont("Microsoft YaHei", 12));
        okButton->setMask(QQFunctions::getRoundedMask(okButton->size(), 5));
        okButton->setFlat(true);
        okButton->setAutoFillBackground(true);
        QPushButton *cancelButton = m_buttonBox->addButton(QString::fromLocal8Bit("取消"), QDialogButtonBox::RejectRole);
        cancelButton->setFixedSize(QSize(100, 40));
        cancelButton->setFont(QFont("Microsoft YaHei", 12));
        cancelButton->setMask(QQFunctions::getRoundedMask(cancelButton->size(), 5));
        cancelButton->setFlat(true);
        QPalette cancelPalette = cancelButton->palette();
        cancelPalette.setColor(QPalette::Button, Qt::red);
        cancelPalette.setColor(QPalette::ButtonText, Qt::white);
        cancelButton->setPalette(cancelPalette);
        cancelButton->setAutoFillBackground(true);

        QVBoxLayout *m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(20, 20, 20, 20);
        m_layout->setSpacing(15);
        m_layout->addWidget(m_appBar);
        m_layout->addWidget(m_tittleLabel);
        m_layout->addWidget(m_buttonBox);
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &MessageDialog::do_accepted);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &MessageDialog::reject);
    }

    MessageDialog::~MessageDialog()
    {
    }

    void MessageDialog::removeNextAllWidget(QLayout *layout)
    {
        QQueue<QLayout *> layoutQueue;
        layoutQueue.enqueue(layout);
        while (!layoutQueue.isEmpty())
        {
            QLayout *nextLayout = layoutQueue.dequeue();
            QLayoutItem *item = nextLayout->takeAt(0);
            while (item != nullptr)
            {
                if (item->widget() != nullptr)
                {
                    nextLayout->removeItem(item);
                    delete item;
                }
                else if (item->spacerItem() != nullptr)
                {
                    nextLayout->removeItem(item);
                    delete item;
                }
                else if (item->layout() != nullptr)
                {
                    layoutQueue.enqueue(item->layout());
                    nextLayout->removeItem(item);
                }
                item = nextLayout->takeAt(0);
            }
            if (nextLayout != layout)
            {
                delete nextLayout;
            }
        }
    }

    void MessageDialog::showEvent(QShowEvent *event)
    {
        if (m_shadowWidget != nullptr)
        {
            m_shadowWidget->setVisible(true);
        }
        return QDialog::showEvent(event);
    }

    void MessageDialog::hideEvent(QHideEvent *event)
    {
        if (m_shadowWidget != nullptr)
        {
            m_shadowWidget->setVisible(false);
        }
        return QDialog::hideEvent(event);
    }

    void MessageDialog::setShadowParent(QWidget *parent)
    {
        m_shadowWidget->setParent(parent);
        m_shadowWidget->setFixedSize(parent->size());
        m_shadowWidget->move(0, 0);
        m_shadowWidget->setVisible(false);
    }

    void MessageDialog::setTittle(const QString &tittle)
    {
        m_tittle = tittle;
        m_tittleLabel->setText(m_tittle);
    }

    void MessageDialog::addWidget(QWidget *widget)
    {
        qobject_cast<QVBoxLayout *>(this->layout())->insertWidget(this->layout()->count() - 1, widget);
    }

    void MessageDialog::removeAllWidget()
    {
        while (this->layout()->count() > 3)
        {
            QLayoutItem *item = this->layout()->takeAt(2);
            if (item->widget() != nullptr)
            {
                this->layout()->removeItem(item);
                delete item->widget();
            }
            else if (item->spacerItem() != nullptr)
            {
                this->layout()->removeItem(item);
                delete item->spacerItem();
            }
            else if (item->layout() != nullptr)
            {
                removeNextAllWidget(item->layout());
                this->layout()->removeItem(item);
                delete item->layout();
            }
            delete item;
        }
    }

    void MessageDialog::setFunction(std::function<void()> &function)
    {
        m_function.emplace();
        m_function = function;
    }

    void MessageDialog::setAlertInformation(const QString &text)
    {
        // 如果正在警告中，不允许再次警告,默认2.5秒后恢复
        m_isAlerting.emplace(false);
        m_alert.emplace();
        m_alert = text;
        QPalette palette = m_tittleLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        m_tittleLabel->setPalette(palette);
        m_tittleLabel->setText(m_alert.value());
        QTimer::singleShot(2500, this, [=]()
                           {
			if (m_isAlerting.has_value())
			{
				QPalette palette = m_tittleLabel->palette();
				palette.setColor(QPalette::WindowText, QColor("#7F7F7F"));
				m_tittleLabel->setPalette(palette);
				m_tittleLabel->setText(m_tittle);
				m_isAlerting = false;
			} });
    }

    void MessageDialog::setOkButtonEnable(bool enable)
    {
        QPushButton *button = qobject_cast<QPushButton *>(m_buttonBox->buttons().at(0));
        if (enable)
        {
            button->setCursor(Qt::ArrowCursor);
            button->setEnabled(true);
            QPalette palette = button->palette();
            palette.setColor(QPalette::Button, QColor("#0099FF"));
            palette.setColor(QPalette::ButtonText, QColor("#FFFFFF"));
            button->setPalette(palette);
        }
        else
        {
            button->setCursor(Qt::ForbiddenCursor);
            button->setEnabled(false);
            QPalette palette = button->palette();
            palette.setColor(QPalette::Button, QColor("#B2E0FF"));
            palette.setColor(QPalette::ButtonText, QColor("#FFFFFF"));
            button->setPalette(palette);
        }
        button->setEnabled(enable);
    }

    void MessageDialog::do_accepted()
    {
        // 如果正在警告中，不允许执行
        if (m_function.has_value())
        {
            m_function.value()();
            if (m_isAlerting.has_value() && m_isAlerting)
            {
                return;
            }
        }
        this->accept();
    }

    StackWidget::StackWidget(QWidget *parent)
        : QStackedWidget(parent)
    {
        m_value = 0;
        m_nextIndex = 0;
        m_isAnimation = false;
        m_direction = QQEnums::DirectionType::lefttoright;
        m_animation = new QPropertyAnimation(this, QByteArray(), this);
        m_animation->setDuration(500);
        connect(m_animation, &QPropertyAnimation::valueChanged, this, &StackWidget::do_animationValueChanged);
        connect(m_animation, &QPropertyAnimation::finished, this, &StackWidget::do_animationFinished);
    }

    StackWidget::~StackWidget()
    {
    }

    void StackWidget::setAnimationCurrentIndex(int index, QQEnums::DirectionType direction)
    {
        emit this->currentChanged(index);
        if (m_isAnimation)
        {
            return;
        }
        this->widget(this->currentIndex())->hide();
        m_nextIndex = index;
        m_direction = direction;
        switch (m_direction)
        {
        case QQEnums::DirectionType::lefttoright:
        {
            m_animation->setStartValue(geometry().width());
            m_animation->setEndValue(0);
            m_animation->start();
            break;
        }
        case QQEnums::DirectionType::righttoleft:
        {
            m_animation->setStartValue(0);
            m_animation->setEndValue(geometry().width());
            m_animation->start();
            break;
        }
        default:
            break;
        }
    }

    void StackWidget::paintEvent(QPaintEvent *event)
    {

        QPainter painter(this);
        QRect baseRect = this->geometry();
        if (m_direction == QQEnums::DirectionType::lefttoright)
        {
            QPixmap tempPixmapOne(this->size());
            this->widget(this->currentIndex())->render(&tempPixmapOne);
            QRectF targetOne(0, 0, m_value, baseRect.height());
            QRectF sourceOne(baseRect.width() - m_value, 0, m_value, baseRect.height());
            painter.drawPixmap(targetOne, tempPixmapOne, sourceOne);

            QPixmap tempPixmapTwo(this->size());
            this->widget(m_nextIndex)->resize(this->size());
            this->widget(m_nextIndex)->render(&tempPixmapTwo);
            QRectF targetTwo(m_value, 0, baseRect.width() - m_value, baseRect.height());
            QRectF sourceTwo(0, 0, baseRect.width() - m_value, baseRect.height());
            painter.drawPixmap(targetTwo, tempPixmapTwo, sourceTwo);
        }
        else if (m_direction == QQEnums::DirectionType::righttoleft)
        {
            QPixmap tempPixmapOne(this->size());
            this->widget(this->currentIndex())->render(&tempPixmapOne);
            QRectF targetOne(m_value, 0, baseRect.width() - m_value, baseRect.height());
            QRectF sourceOne(0, 0, baseRect.width() - m_value, baseRect.height());
            painter.drawPixmap(targetOne, tempPixmapOne, sourceOne);

            QPixmap tempPixmapTwo(this->size());
            this->widget(m_nextIndex)->resize(this->size());
            this->widget(m_nextIndex)->render(&tempPixmapTwo);
            QRectF targetTwo(0, 0, m_value, baseRect.height());
            QRectF sourceTwo(baseRect.width() - m_value, 0, m_value, baseRect.height());
            painter.drawPixmap(targetTwo, tempPixmapTwo, sourceTwo);
        }
    }

    void StackWidget::do_animationValueChanged(const QVariant &value)
    {
        m_value = value.toDouble();
        this->update();
    }

    void StackWidget::do_animationFinished()
    {
        m_isAnimation = false;
        this->setCurrentIndex(m_nextIndex);
        this->update();
    }

    PixelListView::PixelListView(QWidget *parent)
        : QListView(parent)
    {
        this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    PixelListView::~PixelListView()
    {
    }

    void PixelListView::wheelEvent(QWheelEvent *event)
    {
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 0.5 * event->angleDelta().y());
        event->accept();
    }

    void AnimationFillButton::QQProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == PE_IndicatorRadioButton)
        {
            return;
        }
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    void AnimationFillButton::QQProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_RadioButton)
        {
            QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
            if (buttonOption && (buttonOption->state & State_Sunken))
            {
                buttonOption->state &= ~State_Sunken;
                buttonOption->state &= ~State_MouseOver;
            }
            if (buttonOption && (buttonOption->state & State_MouseOver))
            {
                buttonOption->state &= ~State_MouseOver;
                painter->fillRect(option->rect, QColor("#C5C4C5"));
            }
            if (buttonOption && (buttonOption->state & State_On))
            {
                buttonOption->state &= ~State_On;
                painter->fillRect(option->rect, QColor("#C5C4C5"));
            }
        }
        else if (element == CE_RadioButtonLabel)
        {
            QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
            const QRadioButton *radioButton = qobject_cast<const QRadioButton *>(widget);
            if (buttonOption && radioButton)
            {
                QRect rect = option->rect;
                QPixmap pixmap = buttonOption->icon.pixmap(buttonOption->iconSize);
                int x = (radioButton->rect().width() - pixmap.width()) / 2;
                int y = (radioButton->rect().height() - pixmap.height()) / 2;
                painter->drawPixmap(x, y, pixmap);
                return;
            }
        }
        return QProxyStyle::drawControl(element, option, painter, widget);
    }

    AnimationFillButton::AnimationFillButton(QWidget *parent)
        : QRadioButton(parent)
    {
        m_timer = new QTimer(this);
        m_proxyStyle = new QQProxyStyle;
        this->setStyle(m_proxyStyle);
        connect(m_timer, &QTimer::timeout, this, &AnimationFillButton::do_timeout);
        connect(this, &QRadioButton::toggled, this, &AnimationFillButton::do_toggled);
    }

    AnimationFillButton::~AnimationFillButton()
    {
        delete m_proxyStyle;
    }

    void AnimationFillButton::setIconSize(const QSize &size)
    {
        m_prePixmap = this->icon().pixmap(size);
        QImage image = this->icon().pixmap(size).toImage();
        m_pixmap = QPixmap::fromImage(QQFunctions::getFillImage(image, Qt::black, Qt::blue));
        return QRadioButton::setIconSize(size);
    }

    void AnimationFillButton::mouseReleaseEvent(QMouseEvent *event)
    {
        if (this->isEnabled())
        {
            if (event->button() == Qt::LeftButton)
            {
                m_width = 0;
                m_height = 0;
                m_timer->start(5);
            }
        }
        return QRadioButton::mouseReleaseEvent(event);
    }

    void AnimationFillButton::do_timeout()
    {
        if (!this->isChecked())
        {
            m_timer->stop();
            this->setIcon(m_prePixmap);
            return;
        }
        if (m_width < this->iconSize().width())
        {
            m_width += 1;
        }
        else if (m_height < this->iconSize().height())
        {
            m_height += 1;
        }
        else
        {
            m_timer->stop();
        }

        QPainterPath path;
        if (m_width < this->iconSize().width())
        {
            path.moveTo(0, this->iconSize().height());
            path.lineTo(0, this->iconSize().height() - m_width);
            path.lineTo(m_width, this->iconSize().height());
            path.lineTo(0, this->iconSize().height());
            path.closeSubpath();
        }
        else
        {
            path.moveTo(0, this->iconSize().height());
            path.lineTo(0, 0);
            path.lineTo(m_height, 0);
            path.lineTo(this->iconSize().width(), m_height);
            path.lineTo(this->iconSize().width(), this->iconSize().height());
            path.lineTo(0, this->iconSize().height());
            path.closeSubpath();
        }
        QPixmap pixmap = QPixmap(this->iconSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, m_pixmap);
        this->setIcon(pixmap);
    }

    void AnimationFillButton::do_toggled(bool checked)
    {
        if (!checked)
        {
            m_timer->stop();
            this->setIcon(m_prePixmap);
        }
    }

}