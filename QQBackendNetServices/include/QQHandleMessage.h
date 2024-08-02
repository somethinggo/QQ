#ifndef QQ_HANDLE_MESSAGE_H
#define QQ_HANDLE_MESSAGE_H

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qscopedpointer.h>
#include <qprocess.h>
#include "qglobalvalue.h"

class QQHandleMessage
{
public:
    virtual ~QQHandleMessage() = default;
    /**
     * @brief 处理本地客户端消息
     * @param data 消息数据
     */
    virtual void handleLocalClientMessage(const QLocalSocket &socket) = 0;
    /**
     * @brief 处理远程服务器消息
     * @param data 消息数据
     */
    virtual void handleRemoteServerMessage(const QByteArray &data) = 0;
};

class QQHandleMessageV1 : public QQHandleMessage
{
public:
    void handleLocalClientMessage(const QLocalSocket &socket) override;
    void handleRemoteServerMessage(const QByteArray &data) override;
};

class QQHandleMessageFactory
{
public:
    /**
     * @brief 创建消息处理器
     * @param version 版本号
     * @return 消息处理器
     * @note 使用QScopedPointer进行管理
     * @note 如果版本号不匹配，则返回nullptr
     */
    static QScopedPointer<QQHandleMessage> createCalculator(const QString &version)
    {
        if (version == "v1")
        {
            return QScopedPointer<QQHandleMessage>(new QQHandleMessageV1);
        }
        else
        {
            return QScopedPointer<QQHandleMessage>(nullptr);
        }
    }
};

#endif