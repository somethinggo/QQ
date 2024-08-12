#ifndef QQ_COMMON_THEME_H
#define QQ_COMMON_THEME_H

#include <qfont.h>
#include <qicon.h>
#include <qpalette.h>
#include <qbrush.h>
#include <qpen.h>
#include <qcolor.h>
#include <qstyle.h>
#include <qstyleoption.h>

#include "qqdefine.h"

namespace QQThemes
{
    class DefaultTheme
    {
        Q_MSINGLETON_CREATE(DefaultTheme)
        DefaultTheme()
        {
            m_emoji_tabbar_emoji_icon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
            m_emoji_tabbar_emoji_icon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
            m_emoji_tabbar_like_icon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
            m_emoji_tabbar_like_icon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
        }

    public:
        QFont g_font = QFont("Microsoft YaHei");
        QIcon g_icon = QIcon();
        QColor g_line_color = QColor("#E0E0E0");
        qreal g_hover_opacity = 0.5;
        qreal g_pressed_opacity = 0.8;

        QIcon m_emoji_tabbar_emoji_icon;
        QIcon m_emoji_tabbar_like_icon;
        QColor m_emoji_tabbar_pressed = QColor("#0099FF");
        QColor m_emoji_tabbar_hovered = QColor("#C5C4C5");
        QFont m_emojiLab_unicode_font = QFont("Segoe UI Emoji", 20);
        QColor m_emojiLab_pressed = QColor("#0099FF");
        QColor m_emojiLab_hovered = QColor("#C5C4C5");
        QIcon m_emojiLab_append_icon = QIcon(":/emoji/option/images/emoji/option/appendIcon_no.png");
    };

    struct DarkTheme : public DefaultTheme
    {
    };

    struct LightTheme : public DefaultTheme
    {
    };
}

#endif