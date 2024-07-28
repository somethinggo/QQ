#include "QQInformationListWidget.h"

QQInformationListWidget::QQInformationListWidget(QWidget *parent)
	: QWidget(parent)
{
	m_layout = new QVBoxLayout;
	m_layout->setContentsMargins(10, 0, 40, 20);
	m_layout->setSpacing(30);
	this->setLayout(m_layout);
	this->setFont(QFont("Microsoft YaHei", 11));
}

QQInformationListWidget::~QQInformationListWidget()
{
}

void QQInformationListWidget::addMInformation(QLabel *label, QWidget *widget, QIcon icon, ClientEnums::InformationPostionType postion)
{
	QLabel *iconLabel = new QLabel(this);
	if (!icon.isNull())
	{
		iconLabel = new QLabel(this);
		iconLabel->setPixmap(icon.pixmap(20, 20));
	}
	if (postion == ClientEnums::InformationPostionType::left)
	{
		QHBoxLayout *layout = new QHBoxLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(10);
		layout->addWidget(iconLabel);
		layout->addWidget(label);
		QSpacerItem *item = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
		layout->addSpacerItem(item);
		layout->addWidget(widget);
		m_layout->addLayout(layout);
		m_map.insert(m_map.size(), QPair<QLayout *, ClientEnums::InformationPostionType>(layout, ClientEnums::InformationPostionType::left));
	}
	else if (postion == ClientEnums::InformationPostionType::bottom)
	{
		QHBoxLayout *layout = new QHBoxLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(10);
		layout->addWidget(iconLabel);
		layout->addWidget(label);
		m_layout->addLayout(layout);
		m_layout->addWidget(widget);
		m_map.insert(m_map.size(), QPair<QLayout *, ClientEnums::InformationPostionType>(layout, ClientEnums::InformationPostionType::bottom));
	}
}

void QQInformationListWidget::addMInformation(QString name, QWidget *widget, QIcon icon, ClientEnums::InformationPostionType postion)
{
	QLabel *label = new QLabel(this);
	label->setText(name);
	addMInformation(label, widget, icon, postion);
}

QWidget *QQInformationListWidget::getMInformation(int index, ClientEnums::InformationWidgetType type)
{
	auto iter = m_map.begin() + index;
	if (iter == m_map.end())
	{
		return nullptr;
	}
	QWidget *widget = nullptr;
	QLayout *layout = iter.value().first;
	switch (type)
	{
	case ClientEnums::InformationWidgetType::icon:
	{
		if (layout != nullptr)
		{
			QLayoutItem *item = layout->itemAt(0);
			if (item != nullptr && item->widget() != nullptr)
			{
				widget = item->widget();
			}
		}
		break;
	}
	case ClientEnums::InformationWidgetType::label:
	{
		if (layout != nullptr)
		{
			QLayoutItem *item = layout->itemAt(1);
			if (item != nullptr && item->widget() != nullptr)
			{
				widget = item->widget();
			}
		}
		break;
	}
	case ClientEnums::InformationWidgetType::widget:
	{
		if (iter.value().second == ClientEnums::InformationPostionType::left)
		{
			if (layout != nullptr)
			{
				QLayoutItem *item = layout->itemAt(3);
				if (item != nullptr && item->widget() != nullptr)
				{
					widget = item->widget();
				}
			}
		}
		else if (iter.value().second == ClientEnums::InformationPostionType::bottom)
		{
			int index = m_layout->indexOf(layout);
			QLayoutItem *item = m_layout->itemAt(index + 1);
			if (item != nullptr && item->widget() != nullptr)
			{
				widget = item->widget();
			}
		}
		break;
	}
	default:
		break;
	}
	return widget;
}

QList<QWidget *> QQInformationListWidget::getMInformation(int index, ClientEnums::InformationWidgetTypes types)
{
	QList<QWidget *> list;
	for (int i = 0; i < 8; ++i)
	{
		ClientEnums::InformationWidgetType type = static_cast<ClientEnums::InformationWidgetType>(1 << i);
		if (types.testFlag(type))
		{
			QWidget *widget = getMInformation(index, type);
			if (widget)
			{
				list.append(widget);
			}
		}
	}
	return list;
}

void QQInformationListWidget::removeInformation(int index)
{
	auto iter = m_map.begin() + index;
	if (iter == m_map.end())
	{
		return;
	}
	QLayout *layout = iter.value().first;
	if (iter.value().second == ClientEnums::InformationPostionType::left)
	{
		m_layout->removeItem(layout);
		removeNextAllInformation(layout);
	}
	else if (iter.value().second == ClientEnums::InformationPostionType::bottom)
	{
		int index = m_layout->indexOf(layout);
		m_layout->removeItem(layout);
		removeNextAllInformation(layout);
		QLayoutItem *item = m_layout->takeAt(index);
		if (item != nullptr && item->widget() != nullptr)
		{
			m_layout->removeWidget(item->widget());
			delete item->widget();
		}
	}
}

void QQInformationListWidget::removeAllInformation()
{
	removeNextAllInformation(m_layout);
	m_map.clear();
}

void QQInformationListWidget::removeNextAllInformation(QLayout *layout)
{
	QQueue<QLayout *> layoutQueue;
	layoutQueue.enqueue(layout);
	while (!layoutQueue.isEmpty())
	{
		QLayout *nextLayout = layoutQueue.dequeue();
		QLayoutItem *item = nextLayout->takeAt(0);
		while (item != nullptr)
		{
			if (item->widget() != nullptr)
			{
				nextLayout->removeItem(item);
				delete item;
			}
			else if (item->spacerItem() != nullptr)
			{
				nextLayout->removeItem(item);
				delete item;
			}
			else if (item->layout() != nullptr)
			{
				layoutQueue.enqueue(item->layout());
				nextLayout->removeItem(item);
			}
			item = nextLayout->takeAt(0);
		}
		if (nextLayout != layout)
		{
			delete nextLayout;
		}
	}
}
