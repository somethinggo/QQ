#ifndef QQ_COMMON_THEME_H
#define QQ_COMMON_THEME_H

#include <qfont.h>
#include <qicon.h>
#include <qpalette.h>
#include <qbrush.h>
#include <qpen.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qproxystyle.h>
#include <qstyleoption.h>

/**
 * @brief 主题样式主要是针对颜色、字体、图标等进行设置，而大小主要是方便查看与后续的扩展
 */
namespace QQThemes
{
    struct Font
    {
        QFont m_chat_index_name_font = QFont("Microsoft YaHei", 12);
        QFont m_chat_index_message_font = QFont("Microsoft YaHei", 10);
        QFont m_chat_index_time_font = QFont("Microsoft YaHei", 8);
        QFont m_chat_index_count_font = QFont("Microsoft YaHei", 8);

        QFont m_chat_message_name_font = QFont("Microsoft YaHei", 8);
        QFont m_chat_message_text_font = QFont("Microsoft YaHei", 10);
        QFont m_chat_message_emoji_font = QFont("Segoe UI Emoji", 15);
        QFont m_chat_message_file_name_font = QFont("Microsoft YaHei", 10);
        QFont m_chat_message_file_size_font = QFont("Microsoft YaHei", 8);
        QFont m_chat_message_link_font = QFont("Microsoft YaHei", 10);
        QFont m_chat_message_time_font = QFont("Microsoft YaHei", 8);
        QFont m_chat_message_system_font = QFont("Microsoft YaHei", 10);

        QFont m_friend_index_name_font = QFont("Microsoft YaHei", 12);
        QFont m_friend_index_sign_font = QFont("Microsoft YaHei", 10);
        QFont m_friend_add_input_font = QFont("Microsoft YaHei", 10);
        QFont m_friend_add_tabbar_font = QFont("Microsoft YaHei", 10);
        QFont m_friend_add_validate_name_font = QFont("Microsoft YaHei", 10);
        QFont m_friend_add_validate_account_font = QFont("Microsoft YaHei", 8);
        QFont m_friend_add_validate_lab_font = QFont("Microsoft YaHei", 8);
        QFont m_friend_add_validate_button_font = QFont("Microsoft YaHei", 12);

        QFont m_emojiLab_unicode_font = QFont("Segoe UI Emoji", 20);
    };

    struct Color
    {
        QColor m_chat_index_frame_color = Qt::white;
        QColor m_chat_index_frame_hover_color = QColor("#F2F2F2");
        QColor m_chat_index_frame_selected_color = QColor("#0099FF");
        QColor m_chat_index_name_color = Qt::black;
        QColor m_chat_index_name_hover_color = Qt::black;
        QColor m_chat_index_name_selected_color = Qt::white;
        QColor m_chat_index_message_color = Qt::black;
        QColor m_chat_index_message_hover_color = Qt::white;
        QColor m_chat_index_message_selected_color = Qt::white;
        QColor m_chat_index_time_color = Qt::gray;
        QColor m_chat_index_time_hover_color = Qt::gray;
        QColor m_chat_index_time_selected_color = Qt::white;

        QColor m_chat_message_name_color = QColor("#333333");
        QColor m_chat_message_text_color = Qt::black;
        QColor m_chat_message_file_name_color = QColor("#333333");
        QColor m_chat_message_file_size_color = Qt::gray;
        QColor m_chat_message_link_color = QColor("#333333");
        QColor m_chat_message_time_color = Qt::gray;
        QColor m_chat_message_system_color = Qt::gray;

        QColor m_friend_index_parent_hover_color = QColor("#F2F2F2");
        QColor m_friend_index_hover_color = QColor("#F2F2F2");
        QColor m_friend_index_selected_color = QColor("#0099FF");
        QColor m_friend_index_name_color = Qt::black;
        QColor m_friend_index_name_selected_color = Qt::white;
        QColor m_friend_index_sign_color = Qt::gray;
        QColor m_friend_index_sign_selected_color = Qt::white;
        QColor m_friend_add_tabbar_frame_color = QColor("#F5F5F5");
        QColor m_friend_add_tabbar_hover_color = QColor("#F5F5F5");
        QColor m_friend_add_tabbar_selected_color = QColor("#FFFFFF");
        QColor m_friend_add_tabbar_selected_text_color = QColor("#0099FF");

        QColor m_emoji_tabbar_pressed = QColor("#0099FF");
        QColor m_emoji_tabbar_hovered = QColor("#C5C4C5");
        QColor m_emojiLab_pressed = QColor("#0099FF");
        QColor m_emojiLab_hovered = QColor("#C5C4C5");
    };

    struct Icon
    {
        Icon()
        {
            m_emoji_tabbar_emoji_icon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
            m_emoji_tabbar_emoji_icon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
            m_emoji_tabbar_like_icon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
            m_emoji_tabbar_like_icon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
        };
        QIcon m_emoji_tabbar_emoji_icon;
        QIcon m_emoji_tabbar_like_icon;
        QIcon m_emojiLab_append_icon = QIcon(":/emoji/option/images/emoji/option/appendIcon_no.png");

        QIcon m_chat_message_state_icon;
    };

    struct Size
    {
        QSize m_chat_index_icon_size = QSize(50, 50);
        QSize m_chat_index_frame_size = QSize(280, 70);

        QSize m_chat_message_state_icon_size = QSize(30, 30);
        QSize m_chat_message_icon_size = QSize(40, 40);
        QSize m_chat_message_file_icon_size = QSize(50, 50);
        QSize m_chat_message_file_frame_size = QSize(280, 110);
        QSize m_chat_message_image_frame_max_size = QSize(280, 280);
        QSize m_chat_message_image_frame_min_size = QSize(100, 100);

        QSize m_friend_index_icon_size = QSize(50, 50);
        QSize m_friend_add_validate_icon_size = QSize(40, 40);
        QSize m_friend_add_validate_button_size = QSize(100, 40);
    };

    struct Style
    {
        /**
         * @brief 搜索框代理样式
         */
        class SearchProxyStyle : public QProxyStyle
        {
        public:
            void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
            void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
        };
    };
    Q_GLOBAL_STATIC(Style::SearchProxyStyle, searchProxyStyle)

    class DefaultTheme : public Font, public Color, public Icon, public Size, public Style
    {
    public:
        QFont g_font = QFont("Microsoft YaHei");
        QIcon g_icon = QIcon();
        QColor g_line_color = QColor("#E0E0E0");
        qreal g_hover_opacity = 0.5;
        qreal g_pressed_opacity = 0.8;
    };

    struct DarkTheme : public DefaultTheme
    {
    };

    struct LightTheme : public DefaultTheme
    {
    };
}

#endif