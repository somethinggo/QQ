#include "QQLoginWidget.h"

void QMCommonProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_FrameLineEdit)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QMCommonProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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

QQLoginWidget::QQLoginWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQLoginWidgetClass())
{
	ui->setupUi(this);
	this->setFixedSize(QSize(400, 550));
	this->setWindowFlags(Qt::WindowCloseButtonHint);

	m_count = 0;
	m_proxyStyle = new QMCommonProxyStyle;
	m_accountValidator = new QRegularExpressionValidator(QRegularExpression("\\d{0,12}"), this);
	m_numberValidator = new QRegularExpressionValidator(QRegularExpression("1\\d{0,10}"), this);
	m_passwordValidator = new QRegularExpressionValidator(QRegularExpression("[\\da-zA-z$@,\\-.]{0,12}"), this);
	m_progress = new QtMaterialCircularProgress(this);
	m_progress->setProgressType(Material::IndeterminateProgress);
	m_progress->setUseThemeColors(true);
	m_progress->setLineWidth(5);
	m_progress->setSize(20);
	m_progress->setColor(Qt::white);
	m_progress->hide();

	ui->loginAccountInput->setValidator(m_accountValidator);
	ui->loginAccountInput->setMask(getRoundedRect(ui->loginAccountInput->size(), 10));
	ui->loginAccountInput->setStyle(m_proxyStyle);

	ui->loginPasswordInput->setValidator(m_passwordValidator);
	ui->loginPasswordInput->setToolTip(QString::fromLocal8Bit("密码必须包含大小写字母,数字,特殊字符,长度6-12位!!!"));
	ui->loginPasswordInput->setMask(getRoundedRect(ui->loginPasswordInput->size(), 10));
	ui->loginPasswordInput->setStyle(m_proxyStyle);

	ui->loginBtn->setMask(getRoundedRect(ui->loginBtn->size(), 10));
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

	ui->registerNikeNameInput->setMask(getRoundedRect(ui->registerNikeNameInput->size(), 10));
	ui->registerNikeNameInput->setStyle(m_proxyStyle);

	ui->registerPasswordInput->setValidator(m_passwordValidator);
	ui->registerPasswordInput->setToolTip(QString::fromLocal8Bit("密码必须包含大小写字母,数字,特殊字符,长度6-12位!!!"));
	ui->registerPasswordInput->setMask(getRoundedRect(ui->registerPasswordInput->size(), 10));
	ui->registerPasswordInput->setStyle(m_proxyStyle);

	ui->registerNumberInput->setValidator(m_numberValidator);
	ui->registerNumberInput->setToolTip(QString::fromLocal8Bit("手机号码必须以1开头,且为11位!!!"));
	ui->registerNumberInput->setMask(getRoundedRect(ui->registerNumberInput->size(), 10));
	ui->registerNumberInput->setStyle(m_proxyStyle);

	ui->registerBtn->setMask(getRoundedRect(ui->registerBtn->size(), 10));
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
	ui->findPasswordNumberInput->setMask(getRoundedRect(ui->findPasswordNumberInput->size(), 10));
	ui->findPasswordNumberInput->setStyle(m_proxyStyle);

	ui->findPasswordBtn->setMask(getRoundedRect(ui->findPasswordBtn->size(), 10));
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
	connect(this, &QQLoginWidget::sign_showEnbaleBtn, this, &QQLoginWidget::do_showEnableBtn);
	connect(ui->loginAccountInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->loginPasswordInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->registerNikeNameInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->registerPasswordInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->registerNumberInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->findPasswordNumberInput, &QLineEdit::textChanged, this, &QQLoginWidget::do_senderEnableBtn);
	connect(ui->loginBtn, &QPushButton::clicked, this, &QQLoginWidget::do_loginBtnClicked);
	connect(ui->registerBtn, &QPushButton::clicked, this, &QQLoginWidget::do_registerBtnClicked);
	connect(ui->findPasswordBtn, &QPushButton::clicked, this, &QQLoginWidget::do_findPasswordBtnClicked);
	connect(ui->loginBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLoginWidget::do_changedCurrentIndex);
	connect(ui->loginBtnBox->buttons().at(1), &QPushButton::clicked, this, &QQLoginWidget::do_changedCurrentIndex);
	connect(ui->registerBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLoginWidget::do_changedCurrentIndex);
	connect(ui->findPasswordBtnBox->buttons().at(0), &QPushButton::clicked, this, &QQLoginWidget::do_changedCurrentIndex);
}

QQLoginWidget::~QQLoginWidget()
{
	delete m_proxyStyle;
	delete ui;
}

bool QQLoginWidget::eventFilter(QObject *watched, QEvent *event)
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
				QPoint registerNikeNamePos = ui->registerNikeNameInput->mapToGlobal(QPoint(0, 0));
				QPoint registerPasswordPos = ui->registerPasswordInput->mapToGlobal(QPoint(0, 0));
				QPoint registerNumberPos = ui->registerNumberInput->mapToGlobal(QPoint(0, 0));
				QRect registerNikeNameRect(registerNikeNamePos, ui->registerNikeNameInput->size());
				QRect registerPasswordRect(registerPasswordPos, ui->registerPasswordInput->size());
				QRect registerNumberRect(registerNumberPos, ui->registerNumberInput->size());
				if (registerNikeNameRect.contains(pos))
				{
					ui->registerNikeNameInput->setFocus();
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
					ui->registerNikeNameInput->clearFocus();
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

void QQLoginWidget::closeEvent(QCloseEvent *event)
{
	QJsonObject closeDate;
	closeDate.insert("version", "1.0");
	closeDate.insert("sender", "QQLoginWidget");
	closeDate.insert("action", "close");
	m_localSocket.write(QJsonDocument(closeDate).toJson());
	m_localSocket.waitForBytesWritten();
	m_localSocket.disconnect();
	if (m_localSocket.state() != QAbstractSocket::UnconnectedState)
	{
		m_localSocket.waitForDisconnected(3000);
	}
	return QWidget::closeEvent(event);
}

void QQLoginWidget::do_senderEnableBtn()
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
		if (ui->registerNikeNameInput->text() != "")
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
			emit sign_showEnbaleBtn(true, ui->loginBtn);
		}
		else
		{
			emit sign_showEnbaleBtn(false, ui->loginBtn);
		}
		break;
	case 1:
		if (m_count == 3)
		{
			emit sign_showEnbaleBtn(true, ui->registerBtn);
		}
		else
		{
			emit sign_showEnbaleBtn(false, ui->registerBtn);
		}
		break;
	case 2:
		if (m_count == 1)
		{
			emit sign_showEnbaleBtn(true, ui->findPasswordBtn);
		}
		else
		{
			emit sign_showEnbaleBtn(false, ui->findPasswordBtn);
		}
		break;
	default:
		break;
	}
}

void QQLoginWidget::do_showEnableBtn(bool isEnable, QPushButton *button)
{
	button->setEnabled(isEnable);
	if (isEnable)
	{
		QPalette buttonPalette = button->palette();
		buttonPalette.setColor(QPalette::Button, QColor("#0099FF"));
		button->setPalette(buttonPalette);
	}
	else
	{
		QPalette buttonPalette = button->palette();
		buttonPalette.setColor(QPalette::Button, QColor("#9AD8FF"));
		button->setPalette(buttonPalette);
	}
}

void QQLoginWidget::do_changedCurrentIndex()
{
	m_progress->hide();
	int index = ui->stackedWidget->currentIndex();
	if (sender() == ui->loginBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setMCurrentIndex(1, Direction::LeftToRight);
	}
	else if (sender() == ui->loginBtnBox->buttons().at(1))
	{
		ui->stackedWidget->setMCurrentIndex(2, Direction::RightToLeft);
	}
	else if (sender() == ui->registerBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setMCurrentIndex(0, Direction::RightToLeft);
	}
	else if (sender() == ui->findPasswordBtnBox->buttons().at(0))
	{
		ui->stackedWidget->setMCurrentIndex(0, Direction::LeftToRight);
	}
	switch (index)
	{
	case 0:
		ui->loginPasswordInput->clear();
		emit sign_showEnbaleBtn(false, ui->loginBtn);
		break;
	case 1:
		ui->registerNikeNameInput->clear();
		ui->registerPasswordInput->clear();
		ui->registerNumberInput->clear();
		emit sign_showEnbaleBtn(false, ui->registerBtn);
		break;
	case 2:
		ui->findPasswordNumberInput->clear();
		emit sign_showEnbaleBtn(false, ui->findPasswordBtn);
		break;
	default:
		break;
	}
}

void QQLoginWidget::do_loginBtnClicked()
{
	QString account = ui->loginAccountInput->text();
	QString password = ui->loginPasswordInput->text();
	if (account.count() < 6 || account.count() > 12)
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("账号长度必须在6-12位之间!!!请检查输入!!!"), 3000, this);
		return;
	}
	QRegularExpression regex = QRegularExpression("(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[$@,-.])[\\da-zA-z$@,\\-.]{6,12}");
	if (password.count() < 6 || password.count() > 12 || !regex.match(password).hasMatch())
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("密码长度必须在6-12位之间,且包含数字,大写字母,小写字母,特殊符号!!!请检查输入!!!"), 3000, this);
		return;
	}

	ui->loginBtn->setEnabled(false);
	ui->loginBtn->setText(QString::fromLocal8Bit("登录中..."));
	QPoint pos = ui->loginBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->loginBtn->fontMetrics().horizontalAdvance(ui->loginBtn->text());
	pos = pos + QPoint(ui->loginBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->loginBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject loginData;
	loginData.insert("version", "1.0");
	loginData.insert("sender", "QQLoginWidget");
	loginData.insert("action", "login");
	QJsonObject data;
	data.insert("account", account);
	data.insert("password", password);
	loginData.insert("data", data);
	m_localSocket.write(QJsonDocument(loginData).toJson());
	m_localSocket.waitForBytesWritten();

	if (m_localSocket.waitForReadyRead(3000))
	{
		QByteArray data = m_localSocket.readAll();
		QJsonObject answerData = QJsonDocument::fromJson(data).object();
		QJsonObject returnData = answerData["data"].toObject();
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
			ui->loginBtn->setEnabled(true);
			QPalette loginPalette = ui->loginBtn->palette();
			loginPalette.setColor(QPalette::Button, QColor("#0099FF"));
			loginPalette.setColor(QPalette::ButtonText, Qt::white);
			ui->loginBtn->setPalette(loginPalette);
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
	else
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("应用程序崩溃,请从官网下载最新版!!!"));
		this->close();
		qApp->exit(0);
	}
}

void QQLoginWidget::do_registerBtnClicked()
{
	QString nikeName = ui->registerNikeNameInput->text();
	QString password = ui->registerPasswordInput->text();
	QString number = ui->registerNumberInput->text();
	if (nikeName.count() <= 0 || nikeName.count() > 50)
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("昵称长度必须在1-50位之间!!!请检查输入!!!"), 3000, this);
		return;
	}
	QRegularExpression regex = QRegularExpression("(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[$@,-.])[\\da-zA-z$@,\\-.]{6,12}");
	if (password.count() < 6 || password.count() > 12 || !regex.match(password).hasMatch())
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("密码长度必须在6-12位之间,且包含数字,大写字母,小写字母,特殊符号!!!请检查输入!!!"), 3000, this);
		return;
	}
	if (number.count() != 11)
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("手机号码长度必须为11位!!!请检查输入!!!"), 3000, this);
		return;
	}

	ui->registerBtn->setEnabled(false);
	ui->registerBtn->setText(QString::fromLocal8Bit("注册中..."));
	QPoint pos = ui->registerBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->registerBtn->fontMetrics().horizontalAdvance(ui->registerBtn->text());
	pos = pos + QPoint(ui->registerBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->registerBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject registerData;
	registerData.insert("version", "1.0");
	registerData.insert("sender", "QQLoginWidget");
	registerData.insert("action", "register");
	QJsonObject data;
	data.insert("nikeName", nikeName);
	data.insert("password", password);
	data.insert("number", number);
	registerData.insert("data", data);
	m_localSocket.write(QJsonDocument(registerData).toJson());
	m_localSocket.waitForBytesWritten();

	if (m_localSocket.waitForReadyRead(3000))
	{
		QByteArray data = m_localSocket.readAll();
		QJsonObject answerData = QJsonDocument::fromJson(data).object();
		QJsonObject returnData = answerData["data"].toObject();
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
			ui->registerBtn->setEnabled(true);
			QPalette registerPalette = ui->registerBtn->palette();
			registerPalette.setColor(QPalette::Button, QColor("#0099FF"));
			registerPalette.setColor(QPalette::ButtonText, Qt::white);
			ui->registerBtn->setPalette(registerPalette);
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
	else
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("应用程序崩溃,请从官网下载最新版!!!"));
		this->close();
		qApp->exit(0);
	}
}

void QQLoginWidget::do_findPasswordBtnClicked()
{
	QString number = ui->findPasswordNumberInput->text();
	if (number.count() != 11)
	{
		ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("手机号码长度必须为11位!!!请检查输入!!!"), 3000, this);
		return;
	}
	ui->findPasswordBtn->setEnabled(false);
	QPalette m_findPasswordPalette = ui->findPasswordBtn->palette();
	m_findPasswordPalette.setColor(QPalette::Button, QColor("#9AD8FF"));
	m_findPasswordPalette.setColor(QPalette::ButtonText, Qt::white);
	ui->findPasswordBtn->setPalette(m_findPasswordPalette);
	ui->findPasswordBtn->setText(QString::fromLocal8Bit("找回中..."));
	QPoint pos = ui->findPasswordBtn->mapTo(this, QPoint(0, 0));
	int textLength = ui->findPasswordBtn->fontMetrics().horizontalAdvance(ui->findPasswordBtn->text());
	pos = pos + QPoint(ui->findPasswordBtn->width() / 2 - textLength / 2 - m_progress->width() / 2 - m_progress->sizeHint().width() / 2, ui->findPasswordBtn->height() / 2 - m_progress->height() / 2);
	m_progress->move(pos);
	m_progress->show();

	QJsonObject findPasswordData;
	findPasswordData.insert("version", "1.0");
	findPasswordData.insert("sender", "QQLoginWidget");
	findPasswordData.insert("action", "findPassword");
	QJsonObject data;
	data.insert("number", number);
	findPasswordData.insert("data", data);
	m_localSocket.write(QJsonDocument(findPasswordData).toJson());
	m_localSocket.waitForBytesWritten();

	if (m_localSocket.waitForReadyRead(3000))
	{
		m_progress->hide();
		ui->findPasswordBtn->setIcon(QIcon());
		ui->findPasswordBtn->setText(QString::fromLocal8Bit("找回密码"));
		ui->findPasswordBtn->setEnabled(true);
		QPalette findPasswordPalette = ui->findPasswordBtn->palette();
		findPasswordPalette.setColor(QPalette::Button, QColor("#0099FF"));
		findPasswordPalette.setColor(QPalette::ButtonText, Qt::white);
		ui->findPasswordBtn->setPalette(findPasswordPalette);
		QByteArray data = m_localSocket.readAll();
		QJsonObject returnData = QJsonDocument::fromJson(data).object();
		if (returnData["result"] == "fail")
		{
			if (returnData["error"] == "number")
			{
				ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("手机号码不存在!!!请检查输入!!!"), 3000, this);
			}
		}
	}
	else
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("应用程序崩溃,请从官网下载最新版!!!"));
		this->close();
		qApp->exit(0);
	}
}
