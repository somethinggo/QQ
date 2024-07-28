#include "QQChatMessageSingleView.h"

QQChatMessageSingleView::QQChatMessageSingleView(QWidget *parent)
	: QWidget(parent)
{
	this->setMouseTracking(true);
	m_message = nullptr;
	m_isHeightChanged = true;
	m_isSending = false;
	m_movie = new QMovie(this);
	m_movie->setFileName(":/widget/common/images/widget/common/loading.gif");
	m_movie->start();
	connect(m_movie, &QMovie::frameChanged, this, &QQChatMessageSingleView::do_stateIconMovieChanged);
}

QQChatMessageSingleView::~QQChatMessageSingleView()
{
}

void QQChatMessageSingleView::setMMessage(ClientConfigs::MessageBaseConfig *message)
{
	m_message = message;
	QString base64 = QString::fromStdString(GlobalValuesAgent::m_user.m_icon);
	m_icon = QPixmap::fromImage(getBase64ToImage(base64));
	m_icon = getRounedPixmap(m_icon.scaled(QSize(40, 40), Qt::KeepAspectRatio), 20);
	m_name = QString::fromLocal8Bit(GlobalValuesAgent::m_user.m_name.c_str());
	switch (m_message->m_messageType)
	{
	case ClientConfigs::MessageBaseConfig::MessageType::text:
	{
		m_textContent.emplace(QString::fromLocal8Bit(m_message->m_content.c_str()));
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::image:
	{
		QString base64 = QString::fromStdString(m_message->m_content);
		m_imageContent.emplace(QPixmap::fromImage(getBase64ToImage(base64)));
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::file:
	{
		QPair<qreal, QString> pair = getFileFormatSize(QFile(QString::fromLocal8Bit(m_message->m_content.c_str())).size());
		m_textContent.emplace(QString::number(pair.first, 'f', 2) + " " + pair.second);
		m_isDownLoading.emplace(false);
		m_downLoadProgress.emplace(0);
		m_fileMimeType.emplace();
		m_fileIcon.emplace();
		m_watcher.emplace();
		connect(&m_watcher.value(), &QFutureWatcher<QString>::finished, this, &QQChatMessageSingleView::do_futureGetFileMimeType);
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::link:
	{
		m_textContent.emplace(QString::fromLocal8Bit(m_message->m_content.c_str()));
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::emoji:
	{
		m_textContent.emplace(QString::fromStdString(m_message->m_content.c_str()));
		break;
	}
	default:
		break;
	}
	this->update();
}

const ClientConfigs::MessageBaseConfig *QQChatMessageSingleView::getMMessage()
{
	return m_message;
}

void QQChatMessageSingleView::setMIsSending(bool isSending)
{
	m_isSending = isSending;
	this->update();
}

void QQChatMessageSingleView::setMFileIcon(const QString &mimeType)
{
	if (m_fileMimeType == "application/msword")
	{
		m_fileIcon.value().load(":/file/images/file/word.png");
	}
	else if (m_fileMimeType == "undefined")
	{
		m_fileIcon.value().load(":/file/images/file/undefined.png");
	}
	else
	{
		m_fileIcon.value().load(":/file/images/file/undefined.png");
	}
}

void QQChatMessageSingleView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	QSize iconSize(40, 40);

	switch (m_message->m_senderType)
	{
	case ClientConfigs::MessageBaseConfig::SenderType::me:
	{
		painter.save();
		m_iconRect = QRect(QPoint(this->width() - m_iconMargin - iconSize.width(), m_iconMargin), iconSize);
		if (m_icon.isNull())
		{
			QString base64 = QString::fromStdString(GlobalValuesAgent::m_user.m_icon);
			m_icon = getRounedPixmap(QPixmap::fromImage(getBase64ToImage(base64)).scaled(iconSize, Qt::KeepAspectRatio), iconSize.width() / 2);
		}
		painter.drawPixmap(m_iconRect, m_icon);
		painter.restore();
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 9));
		painter.setPen(Qt::gray);
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(GlobalValuesAgent::m_user.m_name.c_str());
		}
		m_nameRect = getCalculateTextRects(m_name, painter.font(), this->width() - 2 * m_contentMargin, true);
		m_nameRect.moveTopLeft(QPoint(this->width() - m_contentMargin - m_nameRect.width(), m_iconMargin));
		painter.drawText(m_nameRect, m_name);
		painter.restore();
		switch (m_message->m_messageType)
		{
		case ClientConfigs::MessageBaseConfig::MessageType::text:
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			painter.setPen(Qt::black);
			painter.setBrush(QBrush(Qt::white));
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromLocal8Bit(m_message->m_content.c_str());
			}
			QRect textRect = getCalculateTextRects(m_textContent.value(), painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
			m_frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
			m_frame.moveTopLeft(QPoint(this->width() - m_contentMargin - m_frame.width(), m_nameRect.bottom() + m_nameMargin));
			textRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			painter.drawText(textRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::image:
		{
			painter.save();
			if (m_imageContent.has_value() && m_imageContent.value().isNull())
			{
				QString base64 = QString::fromStdString(m_message->m_content);
				m_imageContent = QPixmap::fromImage(getBase64ToImage(base64));
			}
			m_imageContent = m_imageContent.value().scaled(m_imageContent.value().size().boundedTo(QSize(this->width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
			m_imageContent = getRounedPixmap(m_imageContent.value(), 10);
			m_frame = QRect(QPoint(this->width() - m_contentMargin - m_imageContent.value().width(), m_nameRect.height() + m_iconMargin + m_nameMargin), m_imageContent.value().size());
			painter.drawPixmap(m_frame, m_imageContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::file:
		{
			painter.save();
			QSize frameSize(280, 110);
			QSize fileIconSize(50, 50);
			m_frame = QRect(QPoint(this->width() - m_contentMargin - frameSize.width(), m_nameRect.height() + m_iconMargin + m_nameMargin), frameSize);
			painter.setFont(QFont("Microsoft YaHei", 10));
			painter.setPen(Qt::black);
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			QFileInfo fileInfo(QString::fromLocal8Bit(m_message->m_content.c_str()));
			QString realFileName = fileInfo.fileName();
			QRect fileNameRect = getCalculateTextRects(realFileName, painter.font(), frameSize.width() - 2 * m_contentPadding - fileIconSize.width() - 12 * m_nameMargin);
			fileNameRect.moveTopLeft(m_frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
			painter.drawText(fileNameRect, realFileName);
			QRect fileIconRect = QRect(QPoint(m_frame.right() - 1.5 * m_contentPadding - fileIconSize.width(), m_frame.top() + 1.5 * m_contentPadding), fileIconSize);
			if (m_fileMimeType.has_value() && !m_fileMimeType.value().isEmpty())
			{
				setMFileIcon(m_fileMimeType.value());
				m_fileIcon = m_fileIcon.value().scaled(fileIconSize);
				painter.drawPixmap(fileIconRect, m_fileIcon.value());
			}
			else
			{
				if (!m_watcher.value().isRunning())
				{
					auto function = [](QString fileName) -> QString
					{
						QFile file(fileName);
						if (!file.open(QIODevice::ReadOnly))
						{
							return QString();
						}
						file.close();
						QMimeType mimeType = QMimeDatabase().mimeTypeForFile(fileName, QMimeDatabase::MatchContent);
						return mimeType.name();
					};
					QFuture<QString> future = QtConcurrent::run(function, QString::fromLocal8Bit(m_message->m_content.c_str()));
					m_watcher.value().setFuture(future);
				}
			}
			painter.setFont(QFont("Microsoft YaHei", 9));
			painter.setPen(Qt::gray);
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				QPair<qreal, QString> pair = getFileFormatSize(QFile(QString::fromLocal8Bit(m_message->m_content.c_str())).size());
				m_textContent = QString::number(pair.first, 'f', 2) + " " + pair.second;
			}
			QRect fileSizeRect = getCalculateTextRects(m_textContent.value(), painter.font(), frameSize.width() - 2 * m_contentPadding);
			fileSizeRect.moveTopLeft(QPoint(m_frame.left() + 1.5 * m_contentPadding, m_frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
			painter.drawText(fileSizeRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::audio:
		{
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::video:
		{
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::link:
		{
			painter.save();
			QFont font = QFont("Microsoft YaHei", 10);
			font.setUnderline(true);
			painter.setFont(font);
			painter.setPen(Qt::blue);
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromLocal8Bit(m_message->m_content.c_str());
			}
			QRect linkRect = getCalculateTextRects(m_textContent.value(), painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
			m_frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
			m_frame.moveTopLeft(QPoint(this->width() - m_contentMargin - m_frame.width(), m_nameRect.bottom() + m_nameMargin));
			linkRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			painter.drawText(linkRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::emoji:
		{
			painter.save();
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromStdString(m_message->m_content.c_str());
			}
			QString emojiType = m_textContent.value().split(",")[0].split(":")[1];
			QString emojiData = m_textContent.value().split(",")[1].split(":")[1];
			if (emojiType == "unicode")
			{
				painter.setFont(QFont("Segoe UI Emoji", 15));
				QRect emojiRect = getCalculateTextRects(emojiData, painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
				m_frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
				m_frame.moveTopLeft(QPoint(this->width() - m_contentMargin - m_frame.width(), m_nameRect.bottom() + m_nameMargin));
				emojiRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
				QPainterPath path;
				path.addRoundedRect(m_frame, 10, 10);
				painter.fillPath(path, Qt::white);
				painter.drawText(emojiRect, emojiData);
			}
			else if (emojiType == "picture")
			{
				QPixmap emoji = QPixmap::fromImage(getBase64ToImage(emojiData));
				emoji = emoji.scaled(emoji.size().boundedTo(QSize(this->width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
				emoji = getRounedPixmap(emoji, 10);
				m_frame = QRect(QPoint(this->width() - m_contentMargin - emoji.width(), m_nameRect.height() + m_iconMargin + m_nameMargin), emoji.size());
				painter.drawPixmap(m_frame, emoji);
			}
			else if (emojiType == "gif")
			{
			}
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::blend:
		{
			break;
		}
		default:
			break;
		}
		if (m_isHeightChanged)
		{
			m_isHeightChanged = false;
			emit sign_heightChanged(m_frame.height() + m_nameRect.height() + m_iconMargin + m_nameMargin);
		}
		QSize stateIconSize(30, 30);
		if (m_isSending)
		{
			m_stateRect = QRect(QPoint(0, 0), stateIconSize);
			m_stateRect.moveBottom(m_frame.bottom());
			m_stateRect.moveLeft(m_frame.left() - stateIconSize.width() - m_nameMargin);
			painter.drawPixmap(m_stateRect, m_stateIcon);
		}
		else if (!m_message->m_isSended)
		{
			m_stateRect = QRect(QPoint(0, 0), stateIconSize.boundedTo(QSize(20, 20)));
			m_stateRect.moveBottom(m_frame.bottom());
			m_stateRect.moveLeft(m_frame.left() - stateIconSize.boundedTo(QSize(20, 20)).width() - m_nameMargin);
			m_stateIcon = QPixmap(":/widget/common/images/widget/common/error.png");
			painter.drawPixmap(m_stateRect, m_stateIcon);
		}
		break;
	}
	case ClientConfigs::MessageBaseConfig::SenderType::she:
	{
		painter.save();
		m_iconRect = QRect(QPoint(m_iconMargin, m_iconMargin), iconSize);
		if (m_icon.isNull())
		{
			QString base64 = QString::fromStdString(GlobalValuesAgent::m_user.m_icon);
			m_icon = getRounedPixmap(QPixmap::fromImage(getBase64ToImage(base64)).scaled(iconSize, Qt::KeepAspectRatio), iconSize.width() / 2);
		}
		painter.drawPixmap(m_iconRect, m_icon);
		painter.restore();
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 9));
		painter.setPen(Qt::gray);
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(GlobalValuesAgent::m_user.m_name.c_str());
		}
		m_nameRect = getCalculateTextRects(m_name, painter.font(), this->width() - 2 * m_contentMargin, true);
		m_nameRect.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
		painter.drawText(m_nameRect, m_name);
		painter.restore();
		switch (m_message->m_messageType)
		{
		case ClientConfigs::MessageBaseConfig::MessageType::text:
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			painter.setPen(Qt::black);
			painter.setBrush(QBrush(Qt::white));
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromLocal8Bit(m_message->m_content.c_str());
			}
			QRect textRect = getCalculateTextRects(m_textContent.value(), painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
			m_frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
			m_frame.moveTopLeft(QPoint(m_contentMargin, m_nameRect.bottom() + m_nameMargin));
			textRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			painter.drawText(textRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::image:
		{
			painter.save();
			if (m_imageContent.has_value() && m_imageContent.value().isNull())
			{
				QString base64 = QString::fromStdString(m_message->m_content);
				m_imageContent = QPixmap::fromImage(getBase64ToImage(base64));
			}
			m_imageContent = m_imageContent.value().scaled(m_imageContent.value().size().boundedTo(QSize(this->width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
			m_imageContent = getRounedPixmap(m_imageContent.value(), 10);
			m_frame = QRect(QPoint(m_contentMargin, m_nameRect.height() + m_iconMargin + m_nameMargin), m_imageContent.value().size());
			painter.drawPixmap(m_frame, m_imageContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::file:
		{
			painter.save();
			QSize frameSize(280, 110);
			QSize fileIconSize(50, 50);
			m_frame = QRect(QPoint(m_contentMargin, m_nameRect.height() + m_iconMargin + m_nameMargin), frameSize);
			painter.setFont(QFont("Microsoft YaHei", 10));
			painter.setPen(Qt::black);
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			QString fileName = QString::fromLocal8Bit(m_message->m_content.c_str());
			QFileInfo fileInfo(fileName);
			QString realFileName = fileInfo.fileName();
			QRect fileNameRect = getCalculateTextRects(realFileName, painter.font(), frameSize.width() - 2 * m_contentPadding - fileIconSize.width() - 12 * m_nameMargin);
			fileNameRect.moveTopLeft(m_frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
			painter.drawText(fileNameRect, realFileName);
			QRect fileIconRect = QRect(QPoint(m_frame.right() - 1.5 * m_contentPadding - fileIconSize.width(), m_frame.top() + 1.5 * m_contentPadding), fileIconSize);
			if (!m_fileMimeType.value().isEmpty())
			{
				setMFileIcon(m_fileMimeType.value());
				m_fileIcon = m_fileIcon.value().scaled(fileIconSize);
				painter.drawPixmap(fileIconRect, m_fileIcon.value());
				if (m_message->m_isDownLoaded.has_value() && !m_message->m_isDownLoaded)
				{
					painter.save();
					QPainterPath iconPath;
					iconPath.addRoundedRect(fileIconRect, 10, 10);
					painter.fillPath(iconPath, QColor(0, 0, 0, 50));
					QRect fileDownLoadIconRect = QRect(fileIconRect.topLeft() + QPoint(2 * m_nameMargin, 2 * m_nameMargin), QSize(30, 30));
					QPen pen;
					pen.setColor(Qt::white);
					pen.setWidth(2);
					painter.setPen(pen);
					painter.drawEllipse(fileDownLoadIconRect);
					painter.drawPixmap(fileDownLoadIconRect, QPixmap(":/file/images/file/download.png"));
					painter.restore();
				}
				else if (m_isDownLoading.has_value() && m_downLoadProgress.has_value() && m_isDownLoading)
				{
					painter.save();
					QPainterPath iconPath;
					iconPath.addRoundedRect(fileIconRect, 10, 10);
					painter.fillPath(iconPath, QColor(0, 0, 0, 50));
					QRect fileCancelDownLoadIconRect = QRect(fileIconRect.topLeft() + QPoint(2 * m_nameMargin, 2 * m_nameMargin), QSize(30, 30));
					QPen pen;
					pen.setColor(Qt::white);
					pen.setWidth(2);
					painter.setPen(pen);
					painter.drawArc(fileCancelDownLoadIconRect, 90 * 16, -m_downLoadProgress.value() * 16);
					painter.drawPixmap(fileCancelDownLoadIconRect, QPixmap(":/file/images/file/cancel.png"));
					painter.restore();
				}
			}
			else
			{
				if (!m_watcher.value().isRunning())
				{
					auto function = [](QString fileName) -> QString
					{
						QFile file(fileName);
						if (!file.open(QIODevice::ReadOnly))
						{
							return QString();
						}
						file.close();
						QMimeType mimeType = QMimeDatabase().mimeTypeForFile(fileName, QMimeDatabase::MatchContent);
						return mimeType.name();
					};
					QFuture<QString> future = QtConcurrent::run(function, fileName);
					m_watcher.value().setFuture(future);
				}
			}
			painter.setFont(QFont("Microsoft YaHei", 9));
			painter.setPen(Qt::gray);
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				QPair<qreal, QString> pair = getFileFormatSize(QFile(QString::fromLocal8Bit(m_message->m_content.c_str())).size());
				m_textContent = QString::number(pair.first, 'f', 2) + " " + pair.second;
			}
			QRect fileSizeRect = getCalculateTextRects(m_textContent.value(), painter.font(), frameSize.width() - 2 * m_contentPadding);
			fileSizeRect.moveTopLeft(QPoint(m_frame.left() + 1.5 * m_contentPadding, m_frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
			painter.drawText(fileSizeRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::audio:
		{
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::video:
		{
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::link:
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			QFont font = painter.font();
			font.setUnderline(true);
			painter.setFont(font);
			painter.setPen(Qt::blue);
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromLocal8Bit(m_message->m_content.c_str());
			}
			QRect linkRect = getCalculateTextRects(m_textContent.value(), painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
			m_frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
			m_frame.moveTopLeft(QPoint(m_contentMargin, m_nameRect.bottom() + m_nameMargin));
			linkRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
			QPainterPath path;
			path.addRoundedRect(m_frame, 10, 10);
			painter.fillPath(path, Qt::white);
			painter.drawText(linkRect, m_textContent.value());
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::emoji:
		{
			painter.save();
			if (m_textContent.has_value() && m_textContent.value().isEmpty())
			{
				m_textContent = QString::fromStdString(m_message->m_content.c_str());
			}
			QString emojiType = m_textContent.value().split(",")[0].split(":")[1];
			QString emojiData = m_textContent.value().split(",")[1].split(":")[1];
			if (emojiType == "unicode")
			{
				painter.setFont(QFont("Segoe UI Emoji", 15));
				QRect emojiRect = getCalculateTextRects(emojiData, painter.font(), this->width() - 2 * m_contentMargin - 2 * m_contentPadding);
				m_frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
				m_frame.moveTopLeft(QPoint(m_contentMargin, m_nameRect.height() + m_iconMargin + m_nameMargin));
				emojiRect.moveTopLeft(m_frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
				QPainterPath path;
				path.addRoundedRect(m_frame, 10, 10);
				painter.fillPath(path, Qt::white);
				painter.drawText(emojiRect, emojiData);
			}
			else if (emojiType == "picture")
			{
				QPixmap emoji = QPixmap::fromImage(getBase64ToImage(emojiData));
				emoji = emoji.scaled(emoji.size().boundedTo(QSize(this->width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
				emoji = getRounedPixmap(emoji, 10);
				m_frame = QRect(QPoint(m_contentMargin, m_nameRect.height() + m_iconMargin + m_nameMargin), emoji.size());
				painter.drawPixmap(m_frame, emoji);
			}
			else if (emojiType == "gif")
			{
			}
			painter.restore();
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::blend:
		{
			break;
		}
		default:
			break;
		}
		if (m_isHeightChanged)
		{
			m_isHeightChanged = false;
			emit sign_heightChanged(m_frame.height() + m_nameRect.height() + m_iconMargin + m_nameMargin);
		}
		break;
	}
	case ClientConfigs::MessageBaseConfig::SenderType::system:
	{
		painter.save();
		painter.setPen(Qt::gray);
		painter.setFont(QFont("Microsoft YaHei", 9));
		QString system = QString::fromLocal8Bit(m_message->m_content.c_str());
		m_frame = getCalculateTextRects(system, painter.font(), this->width() - 2 * m_contentMargin);
		m_frame.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
		m_frame.setWidth(this->width() - 2 * m_contentMargin);
		painter.drawText(m_frame, Qt::AlignCenter, system);
		painter.restore();
		if (m_isHeightChanged)
		{
			m_isHeightChanged = false;
			emit sign_heightChanged(m_frame.height() + m_iconMargin);
		}
		break;
	}
	case ClientConfigs::MessageBaseConfig::SenderType::time:
	{
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 9));
		painter.setPen(Qt::gray);
		QString time = QString::fromStdString(m_message->m_content);
		qint64 daysDiff = QDateTime::fromString(time).secsTo(QDateTime::currentDateTime()) / (24 * 3600);
		if (daysDiff < 1)
		{
			time = QDateTime::fromString(time).toString("HH:mm");
		}
		else if (daysDiff < 2)
		{
			time = "昨天";
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		else if (daysDiff < 7)
		{
			switch (QDateTime::fromString(time).date().dayOfWeek())
			{
			case 1:
				time = "星期一";
				break;
			case 2:
				time = "星期二";
				break;
			case 3:
				time = "星期三";
				break;
			case 4:
				time = "星期四";
				break;
			case 5:
				time = "星期五";
				break;
			case 6:
				time = "星期六";
				break;
			case 7:
				time = "星期日";
				break;
			}
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		else
		{
			time = QDateTime::fromString(time).toString("yyyy/MM/dd");
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		m_frame = getCalculateTextRects(time, painter.font(), this->width() - 2 * m_contentMargin);
		m_frame.moveTopLeft(QPoint(m_contentMargin, 2 * m_iconMargin));
		m_frame.setWidth(this->width() - 2 * m_contentMargin);
		painter.drawText(m_frame, Qt::AlignCenter, time);
		painter.restore();
		if (m_isHeightChanged)
		{
			m_isHeightChanged = false;
			emit sign_heightChanged(m_frame.height() + 2 * m_iconMargin);
		}
		break;
	}
	default:
		break;
	}
}

void QQChatMessageSingleView::resizeEvent(QResizeEvent *event)
{
	// m_isHeightChanged = true;
	return QWidget::resizeEvent(event);
}

void QQChatMessageSingleView::mouseMoveEvent(QMouseEvent *event)
{
	if (m_message == nullptr)
	{
		return QWidget::mouseMoveEvent(event);
	}
	QPoint pos = event->pos();
	switch (m_message->m_messageType)
	{
	case ClientConfigs::MessageBaseConfig::MessageType::link:
	{
		if (m_frame.contains(pos))
		{
			this->setCursor(Qt::PointingHandCursor);
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);
		}
		break;
	}
	default:
		break;
	}
	this->update();
	return QWidget::mouseMoveEvent(event);
}

void QQChatMessageSingleView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_frame.contains(event->pos()) || m_message == nullptr)
		{
			return QWidget::mouseMoveEvent(event);
		}
		switch (m_message->m_messageType)
		{
		case ClientConfigs::MessageBaseConfig::MessageType::file:
		{
			// 发送请求到后台
			/*后台判断文件是否已经下载，
			如果已经下载则异步打开文件，
			如果没有下载则向服务器发送请求下载文件，
			返回状态信息，如果可以下载则异步下载文件，并且更新下载进度，
			如果不可以下载则返回错误信息，取消下载请求，显示错误信息
			*/
			if (!m_message->m_isDownLoaded.has_value())
			{
				return;
			}
			if (m_message->m_isDownLoaded)
			{
			}
			else
			{
			}
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::link:
		{
			// 发送请求到后台
			// 后台异步打开对应的链接查看工具（进程）
			break;
		}
		default:
			break;
		}
	}
	return QWidget::mouseReleaseEvent(event);
}

void QQChatMessageSingleView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_frame.contains(event->pos()) || m_message == nullptr)
		{
			return QWidget::mouseDoubleClickEvent(event);
		}
		switch (m_message->m_messageType)
		{
		case ClientConfigs::MessageBaseConfig::MessageType::image:
		{
			// 发送请求到后台
			// 后台异步打开对应的图片查看工具（进程）
			break;
		}
		default:
			break;
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

void QQChatMessageSingleView::do_stateIconMovieChanged()
{
	m_stateIcon = m_movie->currentPixmap();
	this->update();
}

void QQChatMessageSingleView::do_futureGetFileMimeType()
{
	if (!m_watcher.has_value() || !m_fileMimeType.has_value())
	{
		return;
	}
	m_fileMimeType = m_watcher.value().result();
	if (m_fileMimeType.value().isEmpty())
	{
		m_fileMimeType = "undefined";
	}
	this->update();
}
