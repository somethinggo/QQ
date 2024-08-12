#ifndef QQ_COMMON_WIDGET_H
#define QQ_COMMON_WIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qtabbar.h>
#include <qlabel.h>
#include <qqueue.h>
#include <qdialogbuttonbox.h>
#include <qpropertyanimation.h>
#include <qscreen.h>
#include <qabstractnativeeventfilter.h>
#include <qtimer.h>
#include <qstackedwidget.h>
#include <qpropertyanimation.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <functional>
#include <optional>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#endif

#include <ElaIcon.h>
#include <ElaIconButton.h>

#include "qqfunction.h"

namespace QQWidgets
{

    class AppBar : public QWidget, QAbstractNativeEventFilter
    {
        Q_OBJECT
        Q_MPROPERTY_CREATE(bool, IsAllowDrag, m_isAllowDrag)     // 是否允许拖动
        Q_MPROPERTY_CREATE(bool, IsAllowResize, m_isAllowResize) // 是否允许调整大小
    public:
        explicit AppBar(QWidget *parent = nullptr, QQEnums::AppBarHintType types = QQEnums::AppBarHintType::WindowsHint);
        virtual ~AppBar();
        /**
         * @brief 设置窗口标志
         * @param type 标志类型
         * @param isEnable 是否启用
         */
        void setWindowsFlag(QQEnums::AppBarHintType type, bool isEnable = true);
        /**
         * @brief 设置窗口标志
         * @param types 标志类型
         * @note 该函数会覆盖之前的标志
         */
        void setWindowsFlags(QQEnums::AppBarHintTypes types);
        /**
         * @brief 获取窗口标志
         * @return 窗口标志
         */
        QQEnums::AppBarHintTypes getWindowsFlags();
        /**
         * @brief 设置窗口标题
         * @param title 标题
         * @param flag 对齐方式
         */
        void setWindowsTitle(const QString &title, Qt::AlignmentFlag flag = Qt::AlignmentFlag::AlignCenter);
        /**
         * @brief 设置窗口图标
         * @param icon 图标
         */
        void setWindowsIcon(const QIcon &icon);

    protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
#else
        virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#endif

    private:
        WId m_wid;
        QQEnums::AppBarHintTypes m_types;
        int m_margin;
        QIcon m_icon;
        QLabel *m_iconLabel;
        QString m_title;
        QLabel *m_titleLabel;
        std::optional<ElaIconButton *> m_minimizeButton;
        std::optional<ElaIconButton *> m_closeButton;
        std::optional<ElaIconButton *> m_maximizeButton;
    };

    class AnimationTabBar : public QTabBar
    {
        class QQProxyStyle : public QProxyStyle
        {
        public:
            void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
            void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
        };
        Q_OBJECT
        Q_MPROPERTY_CREATE(qreal, MaxWidth, m_maxWidth)                // 选项卡最大宽度
        Q_MPROPERTY_CREATE(QColor, BackgroundColor, m_backgroundColor) // 背景板颜色
        Q_MPROPERTY_CREATE(QColor, HoverColor, m_hoverColor)           // 鼠标选项卡悬停颜色
        Q_MPROPERTY_CREATE(QColor, SelectColor, m_selectColor)         // 选中选项卡颜色
        Q_MPROPERTY_CREATE(QColor, SelectTextColor, m_selectTextColor) // 选中选项卡文字颜色
    public:
        explicit AnimationTabBar(QWidget *parent = nullptr);
        virtual ~AnimationTabBar();
        /**
         * @brief 设置内部选项卡的外边距
         * @param margin 外边距
         */
        void setMargin(int margin);
        /**
         * @brief 设置内部选项卡的外边距
         * @param left 左边距
         * @param top 上边距
         * @param right 右边距
         * @param bottom 下边距
         */
        void setContentsMargins(int left, int top, int right, int bottom);
        /**
         * @brief 设置内部选项卡的外边距
         * @param margins 外边距
         */
        void setContentsMargins(const QMargins &margins);
        /**
         * @brief 获取内部选项卡的外边距
         * @return 外边距
         */
        QMargins getContentsMargins() const;

    protected:
        QSize tabSizeHint(int index) const override;

    private:
        QProxyStyle *m_proxyStyle;
        QMargins m_margins;
    };

    class MessageDialog : public QDialog
    {
        Q_OBJECT
        Q_MSINGLETON_CREATE(MessageDialog)
    private:
        explicit MessageDialog(QWidget *parent = nullptr);
        virtual ~MessageDialog();

    public:
        /**
         * @brief 设置阴影窗口
         * @param parent 父窗口
         */
        void setShadowParent(QWidget *parent);
        /**
         * @brief 设置标题
         * @param tittle 标题
         */
        void setTittle(const QString &tittle);
        /**
         * @brief 设置确定按钮是否可用
         */
        void setOkButtonEnable(bool enable);
        /**
         * @brief 添加控件
         * @param widget 控件
         */
        void addWidget(QWidget *widget);
        /**
         * @brief 移除所有控件
         */
        void removeAllWidget();
        /**
         * @brief 设置警告信息
         * @param text 文本
         */
        void setAlertInformation(const QString &text = QString());
        /**
         * @brief 设置确定按钮执行函数
         * @param function 函数
         */
        void setFunction(std::function<void()> &function);

    private:
        void removeNextAllWidget(QLayout *layout);

    protected:
        void showEvent(QShowEvent *event) override;
        void hideEvent(QHideEvent *event) override;

    private:
        QWidget *m_shadowWidget;
        AppBar *m_appBar;
        QString m_tittle;
        QLabel *m_tittleLabel;
        QDialogButtonBox *m_buttonBox;
        std::optional<bool> m_isAlerting;
        std::optional<QString> m_alert;
        std::optional<std::function<void()>> m_function;
    private slots:
        void do_accepted();
    };

    class StackWidget : public QStackedWidget
    {
        Q_OBJECT
    public:
        explicit StackWidget(QWidget *parent = nullptr);
        virtual ~StackWidget();
        /**
         * @brief 设置动画切换页面
         * @param index 页面索引
         * @param direction 方向
         */
        void setAnimationCurrentIndex(int index, QQEnums::DirectionType direction = QQEnums::DirectionType::lefttoright);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        QPropertyAnimation *m_animation;
        bool m_isAnimation;
        double m_value;
        int m_nextIndex;
        QQEnums::DirectionType m_direction;
    private slots:
        void do_animationValueChanged(const QVariant &value);
        void do_animationFinished();
    };
}

#endif