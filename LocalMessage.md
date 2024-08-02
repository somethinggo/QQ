## 本地消息协议(json传输)

### 消息基本结构

```json
{
    "version": "协议版本号",
    "sender": "发送者",
    "action": "行为",
    "data":
    {
        "key": "value",
    },
    "extend": "预留扩展"
}
```

#### 登录界面发送登录请求的消息体

```json
{
    "version":"默认版本号",
    "sender":"QQLoginWidget",
    "action":"login",
    "data":
    {
        "account":account,
        "password":password
    }
}
```

#### 登录界面发送注册请求的消息体

```json
{
    "version":"默认版本号",
    "sender":"QQLoginWidget",
    "action":"register",
    "data":
    {
        "nikeName":nikeName,
        "password":password,
        "number":number
    }
}
```

#### 登录界面发送找回请求的消息体

```json
{
    "version":"默认版本号",
    "sender":"QQLoginWiget",
    "action":"findPassword",
    "data":
    {
        "number":number
    }
}
```

