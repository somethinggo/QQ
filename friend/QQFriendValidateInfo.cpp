#include "QQFriendValidateInfo.h"

QQFriendValidateInfo::QQFriendValidateInfo(QWidget *parent)
	: QWidget(parent)
{
	m_user = nullptr;
	m_group = nullptr;
	m_baseInfo = new QWidget(this);
	m_validateList = new QScrollArea(this);
	m_validateList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_validateList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QLayout *moreLayout = new QVBoxLayout;
	moreLayout->setContentsMargins(0, 0, 0, 0);
	moreLayout->setSpacing(10);
	m_validateList->setLayout(moreLayout);
	QLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_baseInfo);
	layout->addWidget(m_validateList);
	this->setLayout(layout);
}

QQFriendValidateInfo::~QQFriendValidateInfo()
{
}

void QQFriendValidateInfo::setBasePeople(QQConfigs::FriendConfig *user)
{
	this->m_user = user;
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(user->m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_add_validate_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_friend_add_validate_icon_size.width() / 2);
	QLabel *iconLab = new QLabel(m_baseInfo);
	iconLab->setFixedSize(QQGlobals::g_theme->m_friend_add_validate_icon_size);
	iconLab->setPixmap(icon);

	QLabel *nameLab = new QLabel(QString::fromStdString(user->m_name), m_baseInfo);
	nameLab->setFont(QQGlobals::g_theme->m_friend_add_validate_name_font);
	QString name = QString::fromStdString(user->m_name);
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_friend_add_validate_name_font, 200, true);
	nameLab->setText(name);

	QLabel *accountLab = new QLabel(QString::fromStdString(user->m_ID), m_baseInfo);
	accountLab->setFont(QQGlobals::g_theme->m_friend_add_validate_account_font);
	QString account = QString::fromStdString(user->m_ID);
	account = QQFunctions::getCalculateText(account, QQGlobals::g_theme->m_friend_add_validate_account_font, 200, true);
	accountLab->setText(account);

	QVBoxLayout *infoLayout = new QVBoxLayout;
	infoLayout->setContentsMargins(0, 0, 0, 0);
	infoLayout->setSpacing(0);
	infoLayout->addWidget(nameLab);
	infoLayout->addWidget(accountLab);

	layout->addWidget(iconLab);
	layout->addLayout(infoLayout);
}

void QQFriendValidateInfo::setBaseGroup(QQConfigs::GroupConfig *group)
{
	this->m_group = group;
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(group->m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_add_validate_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_friend_add_validate_icon_size.width() / 2);
	QLabel *iconLab = new QLabel(m_baseInfo);
	iconLab->setFixedSize(QQGlobals::g_theme->m_friend_add_validate_icon_size);
	iconLab->setPixmap(icon);

	QLabel *nameLab = new QLabel(QString::fromStdString(group->m_name), m_baseInfo);
	nameLab->setFont(QQGlobals::g_theme->m_friend_add_validate_name_font);
	QString name = QString::fromStdString(group->m_name);
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_friend_add_validate_name_font, 200, true);
	nameLab->setText(name);

	QLabel *accountLab = new QLabel(QString::fromStdString(group->m_ID), m_baseInfo);
	accountLab->setFont(QQGlobals::g_theme->m_friend_add_validate_account_font);
	QString account = QString::fromStdString(group->m_ID);
	account = QQFunctions::getCalculateText(account, QQGlobals::g_theme->m_friend_add_validate_account_font, 200, true);
	accountLab->setText(account);

	QVBoxLayout *infoLayout = new QVBoxLayout;
	infoLayout->setContentsMargins(0, 0, 0, 0);
	infoLayout->setSpacing(0);
	infoLayout->addWidget(nameLab);
	infoLayout->addWidget(accountLab);

	layout->addWidget(iconLab);
	layout->addLayout(infoLayout);
}

QPair<QQConfigs::FriendConfig *, QQConfigs::GroupConfig *> QQFriendValidateInfo::getBaseInfo()
{
	return QPair<QQConfigs::FriendConfig *, QQConfigs::GroupConfig *>(m_user, m_group);
}

void QQFriendValidateInfo::addValidate(QLabel *label, QWidget *widget)
{
	QLayout *layout = m_validateList->layout();
	layout->addWidget(label);
	layout->addWidget(widget);
	m_map.insert(m_map.size(), QPair<QLabel *, QWidget *>(label, widget));
}

void QQFriendValidateInfo::addValidate(QString name, QWidget *widget)
{
	QLabel *label = new QLabel(name, m_validateList);
	QPalette palette = label->palette();
	palette.setColor(QPalette::Text, Qt::gray);
	label->setPalette(palette);
	label->setAutoFillBackground(true);
	label->setFont(QQGlobals::g_theme->m_friend_add_validate_lab_font);
	addValidate(label, widget);
}

QPair<QLabel *, QWidget *> QQFriendValidateInfo::getValidate(int index)
{
	auto iter = m_map.find(index);
	if (iter != m_map.end())
	{
		return *iter;
	}
	return QPair<QLabel *, QWidget *>(nullptr, nullptr);
}