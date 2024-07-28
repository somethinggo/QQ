#include "QQSearchHistoryLabel.h"

QQSearchHistoryLabel::QQSearchHistoryLabel(QWidget *parent)
    : QLabel(parent)
{
    m_isPressed = false;
    this->setMouseTracking(true);
}

QQSearchHistoryLabel::~QQSearchHistoryLabel()
{
}

void QQSearchHistoryLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        this->update();
    }
    QLabel::mousePressEvent(event);
}

void QQSearchHistoryLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = false;
        this->update();
        emit sign_userClickSearchHistory(this->text());
    }
    QLabel::mouseReleaseEvent(event);
}

void QQSearchHistoryLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    if (m_isPressed)
    {
        painter.setBrush(QColor("#0099FF"));
    }
    else
    {
        painter.setBrush(QColor("#E5E5E5"));
    }
    painter.drawRoundedRect(this->rect(), 5, 5);
    if (m_isPressed)
    {
        painter.setPen(Qt::white);
    }
    else
    {
        painter.setPen(Qt::black);
    }
    painter.drawText(this->rect(), Qt::AlignCenter, this->text());
}