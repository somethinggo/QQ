#pragma once
// 皮肤描述框
#include <QWidget>
#include <qevent.h>
#include <qlabel.h>
#include <qpropertyanimation.h>
#include <qlayout.h>

#include "qfunction.hpp"

class QMSkinDesSingleView : public QWidget
{
	Q_OBJECT

public:
	QMSkinDesSingleView(QWidget *parent = nullptr);
	virtual ~QMSkinDesSingleView();
	void setMConfig(BaseConfigs::SkinBaseConfig *config);
	const BaseConfigs::SkinBaseConfig *getMConfig();

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void enterEvent(QEnterEvent *event) override;
#else
	void enterEvent(QEvent *event) override;
#endif
	void leaveEvent(QEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

private:
	BaseConfigs::SkinBaseConfig *m_config;
	QLabel *m_pixmapLabel;
	QLabel *m_textLabel;
	QLayout *m_layout;
	QPropertyAnimation *m_animation;
signals:
	void sign_clicked(BaseConfigs::SkinBaseConfig *m_config);
};
