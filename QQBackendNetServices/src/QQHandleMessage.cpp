#include "QQHandleMessage.h"

void QQHandleMessageV1::handleLocalClientMessage(const QLocalSocket &socket)
{
    QByteArray data = const_cast<QLocalSocket &>(socket).readAll();
    QJsonObject json = QJsonDocument::fromJson(data).object();
    QString sender = json.value("sender").toString();
    QString action = json.value("action").toString();
    if (sender == "QQLoginWidget")
    {
        if (action == "login")
        {
            QJsonObject data = json.value("data").toObject();
            QString account = data.value("account").toString();
            QString password = data.value("password").toString();
            // 测试账号密码
            if (account == "123456" && password == "@Ke123456")
            {
                QJsonObject sendData;
                sendData.insert("version", "1.0");
                sendData.insert("sender", "QQBackendNetServices");
                sendData.insert("action", "login-reply");
                QJsonObject data;
                data.insert("result", "success");
                sendData.insert("data", data);
                const_cast<QLocalSocket &>(socket).write(QJsonDocument(sendData).toJson());
                const_cast<QLocalSocket &>(socket).flush();
                QProcess::startDetached(m_localQQDesktopPath);
            }
            // 向服务器发送登录请求
        }
        else if (action == "register")
        {
            QJsonObject data = json.value("data").toObject();
            QString nikeName = data.value("nikeName").toString();
            QString password = data.value("password").toString();
            QString number = data.value("number").toString();
            // 向服务器发送注册请求
        }
        else if (action == "findPassword")
        {
            QJsonObject data = json.value("data").toObject();
            QString number = data.value("number").toString();
            // 向服务器发送找回密码请求
        }
    }
    else if (sender == "QQDesktop")
    {
    }
}

void QQHandleMessageV1::handleRemoteServerMessage(const QByteArray &data)
{
}