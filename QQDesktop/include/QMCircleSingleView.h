#pragma once
// 动态界面的单个动态的显示
#include <qwidget.h>
#include <qicon.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextbrowser.h>
#include <qlistwidget.h>
#include <qboxlayout.h>
#include <qpixmap.h>

#include "qfunction.hpp"

class QMCircleSingleView : public QWidget
{
	Q_OBJECT

public:
	QMCircleSingleView(QWidget *parent = nullptr);
	virtual ~QMCircleSingleView();
	void setMConfig(BaseConfigs::DynamicBaseConfig *config);
	const BaseConfigs::DynamicBaseConfig *getMConfig();

	void connectInit();

private:
	BaseConfigs::DynamicBaseConfig *m_config;
	QBoxLayout *m_layout;
	QLabel *m_iconLabel;
	QLabel *m_nameLabel;
	QTextBrowser *m_textBlock;
	QLabel *m_timeLabel;
	QPushButton *m_likeBtn;
	QPushButton *m_sayBtn;
	QPushButton *m_shareBtn;
	QListWidget *m_commentsListWidget;
	QLineEdit *m_commentEdit;
private slots:
	void do_likeThis(bool isChecked);
	void do_addComment();
};
