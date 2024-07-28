﻿#include "QQLoginWidget.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	m_localSocket.connectToServer("QQBackNetWorkHandleLocalClientServer");
	if (!m_localSocket.waitForConnected(1000))
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("应用程序崩溃,请从官网下载最新版!!!"));
		return 1;
	}
	else
	{
		QJsonObject startData;
		startData.insert("version", "1.0");
		startData.insert("sender", "QQLoginWidget");
		startData.insert("action", "execute");
		QJsonObject data;
		data.insert("result", "success");
		startData.insert("data", data);
		m_localSocket.write(QJsonDocument(startData).toJson());
	}
	QQLoginWidget w;
	w.show();

	return a.exec();
}
