#include "QQEmojiLab.h"

QQEmojiLab::QQEmojiLab(QWidget *parent)
    : QLabel(parent)
{
    m_isHovered = false;
    m_isPressed = false;
}

QQEmojiLab::~QQEmojiLab()
{
}

void QQEmojiLab::setMovie(QMovie *movie)
{
    if (movie != nullptr)
    {
        movie->jumpToFrame(0);
    }
    return QLabel::setMovie(movie);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QQEmojiLab::enterEvent(QEnterEvent *event)
#else
void QQEmojiLab::enterEvent(QEvent *event)
#endif
{
    m_isHovered = true;
    if (this->movie() != nullptr)
    {
        this->movie()->jumpToFrame(0);
        this->movie()->start();
    }
    this->update();
    return QLabel::enterEvent(event);
}

void QQEmojiLab::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    if (this->movie() != nullptr)
    {
        this->movie()->stop();
        this->movie()->jumpToFrame(0);
    }
    this->update();
    return QLabel::leaveEvent(event);
}

void QQEmojiLab::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    this->update();
    return QLabel::mousePressEvent(event);
}

void QQEmojiLab::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    this->update();
    return QLabel::mouseReleaseEvent(event);
}

void QQEmojiLab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
#else
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
#endif
    if (m_isPressed)
    {
        QPainterPath path;
        path.addRoundedRect(this->rect(), 10, 10);
        painter.fillPath(path, QQThemes::DefaultTheme::getInstance()->m_emojiLab_pressed);
    }
    else if (m_isHovered)
    {
        QPainterPath path;
        path.addRoundedRect(this->rect(), 10, 10);
        painter.fillPath(path, QQThemes::DefaultTheme::getInstance()->m_emojiLab_hovered);
    }
    return QLabel::paintEvent(event);
}