#pragma once
// 友人界面的详细信息面板
#include <qwidget.h>
#include <qlabel.h>
#include <qmap.h>
#include <qicon.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qflags.h>

#include "qfunction.hpp"

class QQInformationListWidget : public QWidget
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQInformationListWidget)
public:
	void addMInformation(QLabel *label, QWidget *widget, QIcon icon = QIcon(), ClientEnums::InformationPostionType postion = ClientEnums::InformationPostionType::left); // 添加信息
	void addMInformation(QString name, QWidget *widget, QIcon icon = QIcon(), ClientEnums::InformationPostionType postion = ClientEnums::InformationPostionType::left);	 // 添加信息
	QWidget *getMInformation(int index, ClientEnums::InformationWidgetType type);																						 // 获取信息
	QList<QWidget *> getMInformation(int index, ClientEnums::InformationWidgetTypes types);																				 // 获取信息
	void removeInformation(int index);																																	 // 移除信息
	void removeAllInformation();																																		 // 移除所有信息

private:
	explicit QQInformationListWidget(QWidget *parent = nullptr);
	virtual ~QQInformationListWidget();
	void removeNextAllInformation(QLayout *layout);

private:
	QVBoxLayout *m_layout;
	QMap<int, QPair<QLayout *, ClientEnums::InformationPostionType>> m_map; // 图标，名称，控件，布局，类型
};
