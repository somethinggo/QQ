#include "QMCircleSingleView.h"

QMCircleSingleView::QMCircleSingleView(QWidget *parent)
	: QWidget(parent)
{
	m_layout = new QVBoxLayout(this);
	m_iconLabel = new QLabel(this);
	m_nameLabel = new QLabel(this);
	m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_textBlock = new QTextBrowser(this);
	m_timeLabel = new QLabel(this);
	m_likeBtn = new QPushButton(this);
	m_likeBtn->setCheckable(true);
	m_likeBtn->setIcon(QIcon(":/QQ/images/friend_unlike.png"));
	m_sayBtn = new QPushButton(this);
	m_sayBtn->setIcon(QIcon(":/QQ/images/friendCricle_say.png"));
	m_shareBtn = new QPushButton(this);
	m_shareBtn->setIcon(QIcon(":/QQ/images/friendCricle_shared.png"));
	m_commentsListWidget = new QListWidget(this);
	m_commentEdit = new QLineEdit(this);

	QHBoxLayout *m_headLineLayout = new QHBoxLayout;
	m_headLineLayout->addWidget(m_iconLabel);
	m_headLineLayout->addWidget(m_nameLabel);
	m_headLineLayout->addStretch();
	m_layout->addLayout(m_headLineLayout);

	m_layout->addWidget(m_textBlock);

	QHBoxLayout *m_btnLayout = new QHBoxLayout;
	m_btnLayout->addWidget(m_timeLabel);
	m_btnLayout->addStretch();
	m_btnLayout->addWidget(m_likeBtn);
	m_btnLayout->addWidget(m_sayBtn);
	m_btnLayout->addWidget(m_shareBtn);
	m_layout->addLayout(m_btnLayout);

	m_layout->addWidget(m_commentsListWidget);
	m_layout->addWidget(m_commentEdit);
	connectInit();
}

QMCircleSingleView::~QMCircleSingleView()
{
}

void QMCircleSingleView::setMConfig(BaseConfigs::DynamicBaseConfig *config)
{
	m_config = config;
	BaseConfigs::UserBaseConfig user;
	m_iconLabel->setPixmap(QPixmap(QString::fromStdString(user.m_icon)));
	m_nameLabel->setText(QString::fromStdString(user.m_name));
	m_textBlock->setText(QString::fromStdString(m_config->m_content));
	m_timeLabel->setText(QString::fromStdString(std::to_string(m_config->m_time)));
	for (auto &comment : m_config->m_commentList)
	{
		m_commentsListWidget->addItem(QString::fromStdString(comment));
	}
}

const BaseConfigs::DynamicBaseConfig *QMCircleSingleView::getMConfig()
{
	return m_config;
}

void QMCircleSingleView::connectInit()
{
	connect(m_likeBtn, &QPushButton::clicked, this, &QMCircleSingleView::do_likeThis);
	connect(m_sayBtn, &QPushButton::clicked, [=]()
			{ m_commentEdit->setFocus(); });
	connect(m_commentEdit, &QLineEdit::editingFinished, this, &QMCircleSingleView::do_addComment);
}

void QMCircleSingleView::do_addComment()
{
	if (m_commentsListWidget->isVisible() == false)
	{
		m_commentsListWidget->setVisible(true);
	}
	QString comment = m_commentEdit->text();
	if (!comment.isEmpty())
	{
		m_commentsListWidget->addItem(comment);
		m_commentEdit->clear();
	}
}

void QMCircleSingleView::do_likeThis(bool isChecked)
{
	if (isChecked)
	{
		m_likeBtn->setIcon(QIcon(":/QQ/images/friend_like.png"));
	}
	else
	{
		m_likeBtn->setIcon(QIcon(":/QQ/images/friend_unlike.png"));
	}
}
