#pragma once

#include <qwidget.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qbitmap.h>
#include <qbuffer.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qrandom.h>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <qtconcurrentrun.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <functional>

#include "qconfig.h"

// 系统初始化--检验必要文件夹与文件是否存在,检验配置文件是否存在,检验系统注册是否正常
inline void initSystem()
{
	GlobalValuesAgent::m_user.m_name = "hello";
	GlobalValuesAgent::m_user.m_icon = "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";
}

// 系统退出--资源释放，配置书写
inline void exitSystem()
{
	Q_MSINGLETON_FREE
}

// 查询用户信息
inline ClientConfigs::UserFriend_C *getUserInfo(const QString &ID)
{
	// 向后台发送请求
	QJsonObject searchData;
	searchData.insert("version", "1.0");
	searchData.insert("sender", "QQDesktop");
	searchData.insert("action", "search-user");
	QJsonObject data;
	data.insert("ID", ID);
	searchData.insert("data", data);
	GlobalValuesAgent::m_localSocket.write(QJsonDocument(searchData).toJson());
	if (GlobalValuesAgent::m_localSocket.waitForReadyRead(3000))
	{
		QJsonObject answerData = QJsonDocument::fromJson(GlobalValuesAgent::m_localSocket.readAll()).object();
		if (answerData["result"] == "success")
		{
			QJsonObject userData = answerData["data"].toObject();
			ClientConfigs::UserFriend_C *user = new ClientConfigs::UserFriend_C;
			user->m_ID = userData["ID"].toString().toStdString();
			user->m_account = userData["account"].toString().toStdString();
			user->m_name = userData["name"].toString().toStdString();
			user->m_icon = userData["icon"].toString().toStdString();
			user->m_sign = userData["sign"].toString().toStdString();
			QJsonArray dynamics = userData["dynamics"].toArray();
			for (int i = 0; i < dynamics.size(); i++)
			{
				QJsonObject dynamic = dynamics.at(i).toObject();
				user->m_dynamicList.emplace_back(dynamic["ID"].toString().toStdString());
			}
			return user;
		}
		else
		{
			return nullptr;
		}
	};
}

// 查询群聊信息
inline ClientConfigs::UserGroup_C *getGroupInfo(const QString &ID)
{
	// 向后台发送请求
	QJsonObject searchData;
	searchData.insert("version", "1.0");
	searchData.insert("sender", "QQDesktop");
	searchData.insert("action", "search-group");
	QJsonObject data;
	data.insert("ID", ID);
	searchData.insert("data", data);
	GlobalValuesAgent::m_localSocket.write(QJsonDocument(searchData).toJson());
	if (GlobalValuesAgent::m_localSocket.waitForReadyRead(3000))
	{
		QJsonObject answerData = QJsonDocument::fromJson(GlobalValuesAgent::m_localSocket.readAll()).object();
		if (answerData["result"] == "success")
		{
			QJsonObject groupData = answerData["data"].toObject();
			ClientConfigs::UserGroup_C *group = new ClientConfigs::UserGroup_C;
			group->m_ID = groupData["ID"].toString().toStdString();
			group->m_account = groupData["account"].toString().toStdString();
			group->m_name = groupData["name"].toString().toStdString();
			group->m_icon = groupData["icon"].toString().toStdString();
			group->m_describe = groupData["describe"].toString().toStdString();
			group->m_memberCount = groupData["memberCount"].toInt();
			QJsonArray members = groupData["members"].toArray();
			for (int i = 0; i < members.size(); i++)
			{
				QJsonObject member = members.at(i).toObject();
				group->m_memberList.emplace_back(member["ID"].toString().toStdString());
			}
			return group;
		}
		else
		{
			return nullptr;
		}
	}
}

// 取得圆角图片
inline QPixmap getRounedPixmap(const QPixmap &src, int radius)
{
	if (src.isNull())
	{
		return QPixmap();
	}
	QPixmap pixmap(src.size());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
#else
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
#endif
	QPainterPath path;
	path.addRoundedRect(src.rect(), radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(src.rect(), src);
	return pixmap;
}

// 计算文本矩形并适配矩阵--文本，字体，最大宽度，是否省略
inline QRect getCalculateTextRects(QString &text, const QFont &font, quint64 MAXW = UINT64_MAX, bool elided = false)
{
	QFontMetrics fm(font);
	qreal width = fm.horizontalAdvance(text);
	qreal height = fm.height();
	if (elided)
	{
		text = fm.elidedText(text, Qt::ElideRight, MAXW);
		width = fm.horizontalAdvance(text);
	}
	else if (width > MAXW)
	{
		width = MAXW;
		int start = 0;
		while (start < text.length())
		{
			int end = start;
			while (end < text.length())
			{
				int len = fm.horizontalAdvance(text.mid(start, end - start));
				if (len >= MAXW)
				{
					text.insert(end - 1, "\n");
					height += fm.height();
					break;
				}
				end++;
			}
			start = end;
		}
	}
	return QRect(0, 0, width, height);
}

// 取得圆角蒙版
inline QBitmap getRoundedRect(const QSize &size, qreal radius, ClientEnums::RoundPostionTypes types = ClientEnums::RoundPostionType::all)
{
	QBitmap bitmap(size);
	bitmap.fill(Qt::color0);
	QPainter painter(&bitmap);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	painter.setBrush(Qt::color1);
	QRectF topLeft(0, 0, 2 * radius, 2 * radius);
	QRectF topRight(size.width() - 2 * radius, 0, 2 * radius, 2 * radius);
	QRectF bottomLeft(0, size.height() - 2 * radius, 2 * radius, 2 * radius);
	QRectF bottomRight(size.width() - 2 * radius, size.height() - 2 * radius, 2 * radius, 2 * radius);
	QPainterPath path;
	path.moveTo(0, radius);
	if (types.testFlag(ClientEnums::RoundPostionType::topleft))
	{
		path.arcTo(topLeft, 180, -90);
	}
	else
	{
		path.lineTo(bitmap.rect().topLeft());
		path.lineTo(bitmap.rect().topLeft() + QPoint(radius, 0));
	}
	path.lineTo(bitmap.rect().topRight() + QPoint(-radius, 0));
	if (types.testFlag(ClientEnums::RoundPostionType::topright))
	{
		path.arcTo(topRight, 90, -90);
	}
	else
	{
		path.lineTo(bitmap.rect().topRight());
		path.lineTo(bitmap.rect().topRight() + QPoint(0, radius));
	}
	path.lineTo(bitmap.rect().bottomRight() + QPoint(0, -radius));
	if (types.testFlag(ClientEnums::RoundPostionType::bottomright))
	{
		path.arcTo(bottomRight, 0, -90);
	}
	else
	{
		path.lineTo(bitmap.rect().bottomRight());
		path.lineTo(bitmap.rect().bottomRight() + QPoint(-radius, 0));
	}
	path.lineTo(bitmap.rect().bottomLeft() + QPoint(radius, 0));
	if (types.testFlag(ClientEnums::RoundPostionType::bottomleft))
	{
		path.arcTo(bottomLeft, 270, -90);
	}
	else
	{
		path.lineTo(bitmap.rect().bottomLeft());
		path.lineTo(bitmap.rect().bottomLeft() + QPoint(0, -radius));
	}
	path.lineTo(bitmap.rect().topLeft() + QPoint(0, radius));
	path.closeSubpath();
	painter.drawPath(path);
	return bitmap;
}

// 取得旋转图片
inline QPixmap getRotatePixmap(const QPixmap &src, qreal angle)
{
	QTransform transform;
	transform.rotate(angle);
	return src.transformed(transform, Qt::SmoothTransformation);
}

// 计算文件大小（单位自动转换）--返回大小与单位
inline QPair<qreal, QString> getFileFormatSize(qint64 bytes)
{
	const qint64 KILOBYTE = 1024;
	const qint64 MEGABYTE = 1024 * KILOBYTE;
	const qint64 GIGABYTE = 1024 * MEGABYTE;
	const qint64 TERABYTE = 1024 * GIGABYTE;
	qreal result = bytes;
	QString type = "B";
	if (bytes >= TERABYTE)
	{
		result /= TERABYTE;
		type = "TB";
	}
	else if (bytes >= GIGABYTE)
	{
		result /= GIGABYTE;
		type = "GB";
	}
	else if (bytes >= MEGABYTE)
	{
		result /= MEGABYTE;
		type = "MB";
	}
	else if (bytes >= KILOBYTE)
	{
		result /= KILOBYTE;
		type = "KB";
	}
	return QPair<qreal, QString>(result, type);
}

// 将图片转为Base64编码--返回编码与图片类型
inline QPair<QString, QString> getImageToBase64(QImage &image)
{
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	QImageWriter writer(&buffer, "png");
	if (!writer.write(image))
	{
		return QPair<QString, QString>();
	}
	buffer.close();
	return QPair<QString, QString>(byteArray.toBase64(), "png");
}

// 将图片转为Base64编码--返回编码与图片类型
inline QPair<QString, QString> getImageToBase64(QString &fileName)
{
	QImage image(fileName);
	return getImageToBase64(image);
}

// 将Base64转为图片
inline QImage getBase64ToImage(QString &base64)
{
	QByteArray byteArray = QByteArray::fromBase64(base64.toUtf8());
	QImage image;
	image.loadFromData(byteArray);
	return image;
}

// 生成一段随机文本
inline QString getGenerateRandomText(int min, int max)
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	const int possibleCharactersLength = possibleCharacters.length();
	int realLength = min + rand() % (max - min) + 1;
	QString randomString;
	randomString.reserve(realLength);
	for (int i = 0; i < realLength; ++i)
	{
		int index = QRandomGenerator::global()->bounded(possibleCharactersLength);
		randomString.append(possibleCharacters.at(index));
	}
	return randomString;
}

// 判断鼠标是否在窗口内
inline bool getMouseIsInWidget(QWidget *widget)
{
	if (widget == nullptr || !widget->isVisible())
	{
		return false;
	}
	QPoint pos = QCursor::pos();
	QRect widgetRect = widget->rect().translated(widget->mapToGlobal(QPoint(0, 0)));
	return widgetRect.contains(pos);
}