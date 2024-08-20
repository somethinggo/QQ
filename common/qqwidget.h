#ifndef QQ_COMMON_WIDGET_H
#define QQ_COMMON_WIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qradiobutton.h>
#include <qscrollbar.h>
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
#include <qlistview.h>
#include <functional>
#include <optional>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#else Q_OS_LIUNX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QX11Info>
#endif

#include <ElaIcon.h>
#include <ElaIconButton.h>

#include "qqfunction.h"

namespace QQWidgets
{

    /**
     * @brief 自定义标题栏
     */
    class AppBar : public QWidget, public QAbstractNativeEventFilter
    {
        Q_OBJECT
        QQ_PROPERTY_CREATE(bool, IsAllowDrag)   // 是否允许拖动
        QQ_PROPERTY_CREATE(bool, IsAllowResize) // 是否允许调整大小
    public:
        explicit AppBar(QWidget *parent = nullptr, QQEnums::AppBarHintType types = QQEnums::AppBarHintType::WindowsHint);
        ~AppBar();

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
        bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
#else
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#endif

    private:
        QQEnums::AppBarHintTypes m_types;
        WId m_wid;
        int m_margin;
        QIcon m_icon;
        QLabel *m_iconLabel;
        QString m_title;
        QLabel *m_titleLabel;
        std::optional<ElaIconButton *> m_minimizeButton;
        std::optional<ElaIconButton *> m_closeButton;
        std::optional<ElaIconButton *> m_maximizeButton;
    };

    /**
     * @brief 自定义动画选项卡
     */
    class AnimationTabBar : public QTabBar
    {
        class QQProxyStyle : public QProxyStyle
        {
        public:
            void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
            void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
        };
        Q_OBJECT
        QQ_PROPERTY_CREATE(qreal, MaxWidth)         // 选项卡最大宽度
        QQ_PROPERTY_CREATE(QColor, BackgroundColor) // 背景板颜色
        QQ_PROPERTY_CREATE(QColor, HoverColor)      // 鼠标选项卡悬停颜色
        QQ_PROPERTY_CREATE(QColor, SelectColor)     // 选中选项卡颜色
        QQ_PROPERTY_CREATE(QColor, SelectTextColor) // 选中选项卡文字颜色
    public:
        explicit AnimationTabBar(QWidget *parent = nullptr);
        ~AnimationTabBar();

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

    /**
     * @brief 自定义消息对话框
     */
    class MessageDialog : public QDialog
    {
        Q_OBJECT
        QQ_SINGLETON_CREATE(MessageDialog)
    private:
        explicit MessageDialog(QWidget *parent = nullptr);
        ~MessageDialog();

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

    /**
     * @brief 自定义动画多页窗口
     */
    class StackWidget : public QStackedWidget
    {
        Q_OBJECT
    public:
        explicit StackWidget(QWidget *parent = nullptr);
        ~StackWidget();

        /**
         * @brief 设置动画切换页面
         * @param index 页面索引
         * @param direction 方向
         */
        void setAnimationCurrentIndex(int index, QQEnums::DirectionType direction = QQEnums::DirectionType::lefttoright);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private slots:
        void do_animationValueChanged(const QVariant &value);
        void do_animationFinished();

    private:
        QPropertyAnimation *m_animation;
        bool m_isAnimation;
        double m_value;
        int m_nextIndex;
        QQEnums::DirectionType m_direction;
    };

    /**
     * @brief 自定义像素滚动列表视图
     */
    class PixelListView : public QListView
    {
    public:
        explicit PixelListView(QWidget *parent = nullptr);
        ~PixelListView();

    protected:
        void wheelEvent(QWheelEvent *event) override;
    };

    /**
     * @brief 动画填充按钮
     */
    class AnimationFillButton : public QRadioButton
    {
        class QQProxyStyle : public QProxyStyle
        {
        public:
            void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
            void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
        };
        Q_OBJECT
        QQ_PROPERTY_CREATE(QColor, FillColor) // 填充颜色
    public:
        explicit AnimationFillButton(QWidget *parent = nullptr);
        ~AnimationFillButton();
        /**
         * @brief 设置图标大小
         */
        void setIconSize(const QSize &size);

    protected:
        void mouseReleaseEvent(QMouseEvent *event) override;
    private slots:
        void do_timeout();
        void do_toggled(bool checked);

    private:
        QProxyStyle *m_proxyStyle;
        QTimer *m_timer;
        QPixmap m_prePixmap;
        QPixmap m_pixmap;
        qint64 m_width;
        qint64 m_height;
    };
}

#endif