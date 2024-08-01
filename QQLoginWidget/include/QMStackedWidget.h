#pragma once

#include <qstackedwidget.h>
#include <qpropertyanimation.h>
#include <qpainter.h>
#include <qevent.h>

enum class Direction
{
	LeftToRight,
	RightToLeft,
};

class QMStackedWidget : public QStackedWidget
{
	Q_OBJECT
public:
	QMStackedWidget(QWidget *parent = nullptr);
	~QMStackedWidget();
	void setMCurrentIndex(int index, Direction direction = Direction::LeftToRight);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QPropertyAnimation *m_animation;
	bool m_isAnimation;
	double m_value;
	int m_nextIndex;
	Direction m_direction;
private slots:
	void do_animationValueChanged(const QVariant &value);
	void do_animationFinished();
};