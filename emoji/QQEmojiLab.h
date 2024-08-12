#ifndef QQ_EMOJI_EMOJILAB_H
#define QQ_EMOJI_EMOJILAB_H

#include <qlabel.h>
#include <qmovie.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>

#include "qqtheme.h"

class QQEmojiLab : public QLabel
{
    Q_OBJECT
public:
    QQEmojiLab(QWidget *parent = nullptr);
    virtual ~QQEmojiLab();
    void setMovie(QMovie *movie);

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) override;
#else
    void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isHovered;
    bool m_isPressed;
};

#endif // QQ_EMOJI_EMOJILAB_H