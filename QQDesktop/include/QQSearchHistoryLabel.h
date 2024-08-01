#pragma once

#include <qlabel.h>
#include <qevent.h>
#include <qpainter.h>

class QQSearchHistoryLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QQSearchHistoryLabel(QWidget *parent = nullptr);
    ~QQSearchHistoryLabel() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isPressed;
signals:
    void sign_userClickSearchHistory(QString text);
};