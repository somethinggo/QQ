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

#### data中的数据是多对模式

