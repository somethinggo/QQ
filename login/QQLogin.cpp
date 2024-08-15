#include "QQLogin.h"

void QQLoginProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_FrameLineEdit)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQLoginProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_PushButton)
	{
		if (qobject_cast<QDialogButtonBox *>(widget->parent()) != nullptr)
		{
			QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
			if (buttonOption && (buttonOption->state & State_Sunken))
			{
				buttonOption->state &= ~State_Sunken;
			}
			return QProxyStyle::drawControl(element, option, painter, widget);
		}
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		if (buttonOption && (buttonOption->state & State_Enabled))
		{
			if (buttonOption->state & State_MouseOver)
			{
				buttonOption->state &= ~State_MouseOver;
				int h, s, v;
				QColor color = widget->palette().brush(QPalette::Button).color();
				color.getHsv(&h, &s, &v);
				v = abs((v - 25) % 256);
				color.setHsv(h, s, v);
				painter->fillRect(option->rect, color);
			}
			if (buttonOption->state & State_Sunken)
			{
				buttonOption->state &= ~State_Sunken;
				buttonOption->state &= ~State_MouseOver;
				int h, s, v;
				QColor color = widget->palette().brush(QPalette::Button).color();
				color.getHsv(&h, &s, &v);
				v = abs((v - 50) % 256);
				color.setHsv(h, s, v);
				painter->fillRect(option->rect, color);
			}
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQLogin::QQLogin(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQLoginClass())
{
	ui->setupUi(this);
	this->setFixedSize(QSize(400, 550));
	this->setWindowFlags(Qt::WindowCloseButtonHint);

	m_count = 0;
	m_proxyStyle = new QQLoginProxyStyle;
	m_accountValidator = new QRegularExpressionValidator(QRegularExpression("^[1-9][0-9]{4,10}$"), this);
	m_numberValidator = new QRegularExpressionValidator(QRegularExpression("^(?:(?:\\+|00)86)?1(?:(?:3[\\d])|(?:4[5-79])|(?:5[0-35-9])|(?:6[5-7])|(?:7[0-8])|(?:8[\\d])|(?:9[1589]))\\d{8}$"), this);
	m_passwordValidator = new QRegularExpressionValidator(QRegularExpression("^\\S*(?=\\S{6,12})(?=\\S*\\d)(?=\\S*[A-Z])(?=\\S*[a-z])(?=\\S*[!@#$%^&*? ])\\S*$"), this);
	m_progress = new QtMaterialCircularProgress(this);
	m_progress->setProgressType(Material::IndeterminateProgress);
	m_progress->setUseThemeColors(true);
	m_progress->setLineWidth(5);
	m_progress->setSize(20);
	m_progress->setColor(Qt::white);
	m_progress->hide();

	ui->loginAccountInput->setValidator(m_accountValidator);
	ui->loginAccountInput->setMask(QQFunctions::getRoundedMask(ui->loginAccountInput->size(), 10));
	ui->loginAccountInput->setStyle(m_proxyStyle);

	ui->loginPasswordInput->setValidator(m_passwordValidator);
	ui->loginPasswordInput->setToolTip(QString::fromLocal8Bit("密码必须包含大小写字母,数字,特殊字符,长度6-12位!!!"));
	ui->loginPasswordInput->setMask(QQFunctions::getRoundedMask(ui->loginPasswordInput->size(), 10));
	ui->loginPasswordInput->setStyle(m_proxyStyle);

	ui->loginBtn->setMask(QQFunctions::getRoundedMask(ui->loginBtn->size(), 10));
	ui->loginBtn->setStyle(m_proxyStyle);

	ui->loginBtnBox->addButton(new QPushButton(QString::fromLocal8Bit("注册账号"), this), QDialogButtonBox::ButtonRole::ActionRole);
	ui->loginBtnBox->addButton(new QPushButton(QString::fromLocal8Bit("找回密码"), this), QDialogButtonBox::ButtonRole::ActionRole);
	for (int i = 0; i < ui->loginBtnBox->buttons().count(); ++i)
	{
		QPushButton *button = qobject_cast<QPushButton *>(ui->loginBtnBox->buttons().at(i));
		button->setFlat(true);
		button->setFocusPolicy(Qt::NoFocus);
		button->setStyle(m_proxyStyle);
		button->setCursor(Qt::PointingHandCursor);
		QPalette paletteButton = button->palette();
		paletteButton.setColor(QPalette::ButtonText, Qt::blue);
		paletteButton.setColor(QPalette::Button, Qt::transparent);
		button->setAutoFillBackground(true);
		button->setPalette(paletteButton);
		button->setFont(QFont("Microsoft YaHei", 11));
	}

	ui->registerNicknameInput->setMask(QQFunctions::getRoundedMask(ui->registerNicknameInput->size(), 10));
	ui->registerNicknameInput->setStyle(m_proxyStyle);

	ui->registerPasswordInput->setValidator(m_passwordValidator);
	ui->registerPasswordInput->setToolTip(QString::fromLocal8Bit("密码必须包含大小写字母,数字,特殊字符,长度6-12位!!!"));
	ui->registerPasswordInput->setMask(QQFunctions::getRoundedMask(ui->registerPasswordInput->size(), 10));
	ui->registerPasswordInput->setStyle(m_proxyStyle);

	ui->registerNumberInput->setValidator(m_numberValidator);
	ui->registerNumberInput->setToolTip(QString::fromLocal8Bit("手机号码必须以1开头,且为11位!!!"));
	ui->registerNumberInput->setMask(QQFunctions::getRoundedMask(ui->registerNumberInput->size(), 10));
	ui->registerNumberInput->setStyle(m_proxyStyle);

	ui->registerBtn->setMask(QQFunctions::getRoundedMask(ui->registerBtn->size(), 10));
	ui->registerBtn->setStyle(m_proxyStyle);

	ui->registerBtnBox->addButton(new QPushButton(QString::fromLocal8Bit("返回登录"), this), QDialogButtonBox::ButtonRole::ActionRole);
	for (int i = 0; i < ui->registerBtnBox->buttons().count(); ++i)
	{
		QPushButton *button = qobject_cast<QPushButton *>(ui->registerBtnBox->buttons().at(i));
		button->setFlat(true);
		button->setFocusPolicy(Qt::NoFocus);
		button->setStyle(m_proxyStyle);
		button->setCursor(Qt::PointingHandCursor);
		QPalette m_paletteButton = button->palette();
		m_paletteButton.setColor(QPalette::ButtonText, Qt::blue);
		m_paletteButton.setColor(QPalette::Button, Qt::transparent);
		button->setAutoFillBackground(true);
		button->setPalette(m_paletteButton);
		button->setFont(QFont("Microsoft YaHei", 11));
	}

	ui->findPasswordNumberInput->setValidator(m_numberValidator);
	ui->findPasswordNumberInput->setToolTip(QString::fromLocal8Bit("手机号码必须以1开头,且为11位!!!"));
	ui->findPasswordNumberInput->setMask(QQFunctions::getRoundedMask(ui->findPasswordNumberInput->size(), 10));
	ui->findPasswordNumberInput->setStyle(m_proxyStyle);

	ui->findPasswordBtn->setMask(QQFunctions::getRoundedMask(ui->findPasswordBtn->size(), 10));
	ui->findPasswordBtn->setStyle(m_proxyStyle);

	ui->findPasswordBtnBox->addButton(new QPushButton(QString::fromLocal8Bit("返回登录"), this), QDialogButtonBox::ButtonRole::ActionRole);
	for (int i = 0; i < ui->findPasswordBtnBox->buttons().count(); ++i)
	{
		QPushButton *button = qobject_cast<QPushButton *>(ui->findPasswordBtnBox->buttons().at(i));
		button->setFlat(true);
		button->setFocusPolicy(Qt::NoFocus);
		button->setStyle(m_proxyStyle);
		button->setCursor(Qt::PointingHandCursor);
		QPalette m_paletteButton = button->palette();
		m_paletteButton.setColor(QPalette::ButtonText, Qt::blue);
		m_paletteButton.setColor(QPalette::Button, Qt::transparent);
		button->setAutoFillBackground(true);
		button->setPalette(m_paletteButton);
		button->setFont(QFont("Microsoft YaHei", 11));
	}

	ui->stackedWidget->setCurrentIndex(0);

	qApp->installEventFilter(this);
	connect(ui->loginAccountInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->loginPasswordInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->registerNicknameInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->registerPasswordInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->registerNumberInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->findPasswordNumberInput, &QLineEdit::textChanged, this, &QQLogin::do_senderEnableBtn);
	connect(ui->loginBtn, &QPushButton::clicked, this, &QQLogin::do_loginBtnClicked);
	connect(ui->registerBtn, &QPushButton::clicked, this, &QQLogin::do_registerBtnClicked);
	connect(ui->findPasswordBtn, &QPushButton::clicked, this, &QQLogin::do_findPasswordBtnClicked);
	connect(ui->loginBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLogin::do_changedCurrentIndex);
	connect(ui->loginBtnBox->buttons().at(1), &QPushButton::clicked, this, &QQLogin::do_changedCurrentIndex);
	connect(ui->registerBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLogin::do_changedCurrentIndex);
	connect(ui->findPasswordBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLogin::do_changedCurrentIndex);
}

QQLogin::~QQLogin()
{
	delete m_proxyStyle;
	delete ui;
}

void QQLogin::disEnableAllWidget(bool isEnable)
{
	ui->loginAccountInput->setEnabled(isEnable);
	ui->loginPasswordInput->setEnabled(isEnable);
	ui->loginBtn->setEnabled(isEnable);
	ui->loginBtnBox->setEnabled(isEnable);
	ui->registerNicknameInput->setEnabled(isEnable);
	ui->registerPasswordInput->setEnabled(isEnable);
	ui->registerNumberInput->setEnabled(isEnable);
	ui->registerBtn->setEnabled(isEnable);
	ui->registerBtnBox->setEnabled(isEnable);
	ui->findPasswordNumberInput->setEnabled(isEnable);
	ui->findPasswordBtn->setEnabled(isEnable);
	ui->findPasswordBtnBox->setEnabled(isEnable);
}

void QQLogin::login(const QByteArray &data)
{
	QJsonObject loginData = QJsonDocument::fromJson(data).object();
	QJsonObject returnData = loginData["data"].toObject();
	if (returnData["result"] == "success")
	{
		this->close();
		qApp->exit(0);
	}
	else if (returnData["result"] == "fail")
	{
		m_progress->hide();
		ui->loginBtn->setIcon(QIcon());
		ui->loginBtn->setText(QString::fromLocal8Bit("登录"));
		if (returnData["error"] == "account")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("账号不存在!!!请检查输入!!!"), 3000, this);
		}
		else if (returnData["error"] == "password")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("密码错误!!!请检查输入!!!"), 3000, this);
		}
	}
	else
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("应用程序崩溃,请从官网下载最新版!!!"));
		this->close();
		qApp->exit(0);
	}
}

void QQLogin::registered(const QByteArray &data)
{
	QJsonObject returnData = QJsonDocument::fromJson(data).object();
	if (returnData["result"] == "success")
	{
		this->close();
		qApp->exit(0);
	}
	else if (returnData["state"] == "reject")
	{
		m_progress->hide();
		ui->registerBtn->setIcon(QIcon());
		ui->registerBtn->setText(QString::fromLocal8Bit("注册"));
		if (returnData["error"] == "nikeName")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("昵称违规!!!请检查输入!!!"), 3000, this);
		}
		else if (returnData["error"] == "number")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("手机号码不存在!!!请检查输入!!!"), 3000, this);
		}
		else if (returnData["error"] == "password")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("密码不符合要求!!!请检查输入!!!"), 3000, this);
		}
	}
}

void QQLogin::findPassword(const QByteArray &data)
{
	m_progress->hide();
	ui->findPasswordBtn->setIcon(QIcon());
	ui->findPasswordBtn->setText(QString::fromLocal8Bit("找回密码"));
	QJsonObject returnData = QJsonDocument::fromJson(data).object();
	if (returnData["result"] == "fail")
	{
		if (returnData["error"] == "number")
		{
			ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("手机号码不存在!!!请检查输入!!!"), 3000, this);
		}
	}
}

bool QQLogin::event(QEvent *event)
{
	switch (event->type())
	{
		Q_MHANDLE_EVENT(QQEnums::accpectlogin, login);
		Q_MHANDLE_EVENT(QQEnums::accpectregistered, registered);
		Q_MHANDLE_EVENT(QQEnums::accpectfindpassword, findPassword);
	default:
		break;
	}
	return QWidget::event(event);
}

bool QQLogin::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			QPoint pos = mouseEvent->globalPos();
			int index = ui->stackedWidget->currentIndex();
			if (index == 0)
			{
				QPoint loginAccountPos = ui->loginAccountInput->mapToGlobal(QPoint(0, 0));
				QPoint loginPasswordPos = ui->loginPasswordInput->mapToGlobal(QPoint(0, 0));
				QRect loginAccountRect(loginAccountPos, ui->loginAccountInput->size());
				QRect loginPasswordRect(loginPasswordPos, ui->loginPasswordInput->size());
				if (loginAccountRect.contains(pos))
				{
					ui->loginAccountInput->setFocus();
				}
				else if (loginPasswordRect.contains(pos))
				{
					ui->loginPasswordInput->setFocus();
				}
				else
				{
					ui->loginAccountInput->clearFocus();
					ui->loginPasswordInput->clearFocus();
				}
			}
			else if (index == 1)
			{
				QPoint registerNikeNamePos = ui->registerNicknameInput->mapToGlobal(QPoint(0, 0));
				QPoint registerPasswordPos = ui->registerPasswordInput->mapToGlobal(QPoint(0, 0));
				QPoint registerNumberPos = ui->registerNumberInput->mapToGlobal(QPoint(0, 0));
				QRect registerNikeNameRect(registerNikeNamePos, ui->registerNicknameInput->size());
				QRect registerPasswordRect(registerPasswordPos, ui->registerPasswordInput->size());
				QRect registerNumberRect(registerNumberPos, ui->registerNumberInput->size());
				if (registerNikeNameRect.contains(pos))
				{
					ui->registerNicknameInput->setFocus();
				}
				else if (registerPasswordRect.contains(pos))
				{
					ui->registerPasswordInput->setFocus();
				}
				else if (registerNumberRect.contains(pos))
				{
					ui->registerNumberInput->setFocus();
				}
				else
				{
					ui->registerNicknameInput->clearFocus();
					ui->registerPasswordInput->clearFocus();
					ui->registerNumberInput->clearFocus();
				}
			}
			else if (index == 2)
			{
				QPoint findPasswordNumberPos = ui->findPasswordNumberInput->mapToGlobal(QPoint(0, 0));
				QRect findPasswordNumberRect(findPasswordNumberPos, ui->findPasswordNumberInput->size());
				if (findPasswordNumberRect.contains(pos))
				{
					ui->findPasswordNumberInput->setFocus();
				}
				else
				{
					ui->findPasswordNumberInput->clearFocus();
				}
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}

void QQLogin::do_senderEnableBtn()
{
	m_count = 0;
	int index = ui->stackedWidget->currentIndex();
	if (index == 0)
	{
		if (ui->loginAccountInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
		if (ui->loginPasswordInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
	}
	else if (index == 1)
	{
		if (ui->registerNicknameInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
		if (ui->registerPasswordInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
		if (ui->registerNumberInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
	}
	else if (index == 2)
	{
		if (ui->findPasswordNumberInput->text() != "")
		{
			++m_count;
		}
		else
		{
			--m_count;
		}
	}

	switch (index)
	{
	case 0:
		if (m_count == 2)
		{
			ui->loginBtn->setEnabled(true);
		}
		else
		{
			ui->loginBtn->setEnabled(false);
		}
		break;
	case 1:
		if (m_count == 3)
		{
			ui->registerBtn->setEnabled(true);
		}
		else
		{
			ui->registerBtn->setEnabled(false);
		}
		break;
	case 2:
		if (m_count == 1)
		{
			ui->findPasswordBtn->setEnabled(true);
		}
		else
		{
			ui->findPasswordBtn->setEnabled(false);
		}
		break;
	default:
		break;
	}
}

void QQLogin::do_changedCurrentIndex()
{
	m_progress->hide();
	int index = ui->stackedWidget->currentIndex();
	if (sender() == ui->loginBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setAnimationCurrentIndex(1);
	}
	else if (sender() == ui->loginBtnBox->buttons().at(1))
	{
		ui->stackedWidget->setAnimationCurrentIndex(2, QQEnums::DirectionType::righttoleft);
	}
	else if (sender() == ui->registerBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setAnimationCurrentIndex(0, QQEnums::DirectionType::righttoleft);
	}
	else if (sender() == ui->findPasswordBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setAnimationCurrentIndex(0);
	}
	switch (index)
	{
	case 0:
		ui->loginPasswordInput->clear();
		ui->loginBtn->setEnabled(false);
		break;
	case 1:
		ui->registerNicknameInput->clear();
		ui->registerPasswordInput->clear();
		ui->registerNumberInput->clear();
		ui->registerBtn->setEnabled(false);
		break;
	case 2:
		ui->findPasswordNumberInput->clear();
		ui->findPasswordBtn->setEnabled(false);
		break;
	default:
		break;
	}
}

void QQLogin::do_loginBtnClicked()
{
	QString account = ui->loginAccountInput->text();
	QString password = ui->loginPasswordInput->text();

	disEnableAllWidget(false);
	ui->loginBtn->setText(QString::fromLocal8Bit("登录中..."));
	QPoint pos = ui->loginBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->loginBtn->fontMetrics().horizontalAdvance(ui->loginBtn->text());
	pos = pos + QPoint(ui->loginBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->loginBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject loginData;
	loginData.insert("version", "1.0");
	loginData.insert("sender", "login");
	loginData.insert("action", "login");
	QJsonObject data;
	data.insert("account", account);
	data.insert("password", password);
	loginData.insert("data", data);

	Q_MSEND_EVENT(QQEnums::login, QJsonDocument(loginData).toJson());
}

void QQLogin::do_registerBtnClicked()
{
	QString nickname = ui->registerNicknameInput->text();
	QString password = ui->registerPasswordInput->text();
	QString number = ui->registerNumberInput->text();

	disEnableAllWidget(false);
	ui->registerBtn->setText(QString::fromLocal8Bit("注册中..."));
	QPoint pos = ui->registerBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->registerBtn->fontMetrics().horizontalAdvance(ui->registerBtn->text());
	pos = pos + QPoint(ui->registerBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->registerBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject registerData;
	registerData.insert("version", "1.0");
	registerData.insert("sender", "login");
	registerData.insert("action", "register");
	QJsonObject data;
	data.insert("nickname", nickname);
	data.insert("password", password);
	data.insert("number", number);
	registerData.insert("data", data);

	Q_MSEND_EVENT(QQEnums::registered, QJsonDocument(registerData).toJson());
}

void QQLogin::do_findPasswordBtnClicked()
{
	QString number = ui->findPasswordNumberInput->text();

	disEnableAllWidget(false);
	ui->findPasswordBtn->setText(QString::fromLocal8Bit("找回中..."));
	QPoint pos = ui->findPasswordBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->findPasswordBtn->fontMetrics().horizontalAdvance(ui->findPasswordBtn->text());
	pos = pos + QPoint(ui->findPasswordBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->findPasswordBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject findPasswordData;
	findPasswordData.insert("version", "1.0");
	findPasswordData.insert("sender", "login");
	findPasswordData.insert("action", "findPassword");
	QJsonObject data;
	data.insert("number", number);
	findPasswordData.insert("data", data);

	Q_MSEND_EVENT(QQEnums::findpassword, QJsonDocument(findPasswordData).toJson());
}
