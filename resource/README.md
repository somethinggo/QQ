# 模块消息通信参考表

##  消息体基本结构

```json
{
    "version":"版本号",
    "sender":"发送者",
    "receiver":"接受者",
    "action":"行为",
    "data":
    {
        "key":"value"
    },
    "extend":"扩展"
}
```

> 注意：消息体默认使用json传输，异步发送，其中数据段可以使用多个键值对，扩展段是消息体的扩展，可以不使用（程序本身也不会解析）

## 消息体类型参考

| 名称             | 用途                   | 备注                                   |
| ---------------- | ---------------------- | -------------------------------------- |
| `sendinfo`       | 发送基本信息           |                                        |
| `provideinfo`    | 给予(返回)基本信息     |                                        |
| `sendmessage`    | 发送消息体             |                                        |
| `providemessage` | 给予(返回)消息体       | 不仅包括返回消息体，还有对消息体的变更 |
| `loadstorage`    | 加载本地存储数据       |                                        |
| `providestorage` | 给予(返回)本地存储数据 |                                        |
| `savestorage`    | 存储到本地             |                                        |
| `requestmodel`   | 请求模块               | 暂时用作其余未定义消息的类型           |
| `statechange`    | 状态变更               |                                        |

## 1.0

### 用户模块

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2">sendinfo</td>
            <td rowspan="2">network</td>
            <td rowspan="2">login</td>
            <td>(QString)account</td>
        </tr>
        <tr>
            <td>(QString)password</td>
        </tr>
        <tr>
            <td rowspan="3">sendinfo</td>
            <td rowspan="3">network</td>
            <td rowspan="3">registe</td>
            <td>(QString)nickname</td>
        </tr>
        <tr>
            <td>(QString)password</td>
        </tr>
        <tr>
            <td>(QString)number</td>
        </tr>
        <tr>
            <td>sendinfo</td>
            <td>network</td>
            <td>findpassword</td>
            <td>(QString)number</td>
        </tr>
    </tbody>
</table>
### 聊天模块

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loadindex</td>
            <td></td>
        </tr>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loadmessage</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td rowspan="6">savetorage</td>
            <td rowspan="6">storage</td>
            <td rowspan="6">addmessage</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(enum)type</td>
        </tr>
        <tr>
            <td>(QString)sender</td>
        </tr>
        <tr>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td>(qint64)time</td>
        </tr>
        <tr>
            <td rowspan="3">requestmodel</td>
            <td rowspan="3">storage</td>
            <td rowspan="3">updatemessage</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(bool)sended</td>
        </tr>
        <tr>
            <td>(bool)recall</td>
        </tr>
        <tr>
            <td rowspan="3">requestmodel</td>
            <td rowspan="3">file</td>
            <td rowspan="3">sendfile</td>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td>(QString)filename</td>
        </tr>
        <tr>
            <td>(qint64)time</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>audio</td>
            <td>sendaudio</td>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td rowspan="4">sendmessage</td>
            <td rowspan="4">network</td>
            <td rowspan="4">sendmessage</td>
            <td>(enum)type</td>
        </tr>
        <tr>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td>(qint64)time</td>
        </tr>
        <tr>
            <td rowspan="2">requestmodel</td>
            <td rowspan="2">system</td>
            <td rowspan="2">openmessage</td>
            <td>(QString)type</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
    </tbody>
</table>

### 好友模块

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loadindex</td>
            <td></td>
        </tr>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loadinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td rowspan="6">savestorage</td>
            <td rowspan="6">storage</td>
            <td rowspan="6">addinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(QString)account</td>
        </tr>
        <tr>
            <td>(QString)name</td>
        </tr>
        <tr>
            <td>(QString)nickname</td>
        </tr>
        <tr>
            <td>(QIcon)icon</td>
        </tr>
        <tr>
            <td>(QString)sign</td>
        </tr>
        <tr>
            <td rowspan="5">requestmodel</td>
            <td rowspan="5">storage</td>
            <td rowspan="5">updateinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(QString)name</td>
        </tr>
        <tr>
            <td>(QString)nickname</td>
        </tr>
        <tr>
            <td>(QIcon)icon</td>
        </tr>
        <tr>
            <td>(QString)sign</td>
        </tr>
        <tr>
            <td >requestmodel</td>
            <td>storage</td>
            <td>deleteinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>network</td>
            <td>searchinfo</td>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td rowspan="2">requestmodel</td>
            <td rowspan="2">network</td>
            <td rowspan="2">addinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(QString)validates</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>network</td>
            <td>deleteinfo</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>chat</td>
            <td>addchat</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td rowspan="2">requestmodel</td>
            <td rowspan="2">system</td>
            <td rowspan="2">notice</td>
            <td>(enum)type</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
    </tbody>
</table>

### 表情模块

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loademoji</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan="2">savestorage</td>
            <td rowspan="2">storage</td>
            <td rowspan="2">addemoji</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>storage</td>
            <td>deleteemoji</td>
            <td>(QString)ID</td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>chat</td>
            <td>sendemoji</td>
            <td>(QString)content</td>
        </tr>
    </tbody>
</table>

### 音频管理

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="4">sendmessage</td>
            <td rowspan="4">network</td>
            <td rowspan="4">sendaudio</td>
            <td>(enum)type[audio]</td>
        </tr>
        <tr>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td>(qint64)time</td>
        </tr>
        <tr>
            <td rowspan="2">requestmodel</td>
            <td rowspan="2">chat</td>
            <td rowspan="2">updatemessage</td>
            <td>(uint64_t)duration</td>
        </tr>
        <tr>
            <td>(qint64[])volumes</td>
        </tr>
    </tbody>
</table>


### 文件管理

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="4">sendmessage</td>
            <td rowspan="4">network</td>
            <td rowspan="4">sendfile</td>
            <td>(enum)type[file]</td>
        </tr>
        <tr>
            <td>(QString)receiver</td>
        </tr>
        <tr>
            <td>(QString)content</td>
        </tr>
        <tr>
            <td>(qint64)time</td>
        </tr>
    </tbody>
</table>

### 设置与偏好

<table>
    <thead>
        <tr>
            <th>类型</th>
            <th>接受者</th>
            <th>行为</th>
            <th>数据</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>loadstorage</td>
            <td>storage</td>
            <td>loadset</td>
            <td></td>
        </tr>
        <tr>
            <td>requestmodel</td>
            <td>storage</td>
            <td>updateset</td>
            <td></td>
        </tr>
    </tbody>
</table>


### 网络通信

### 系统交互

### 本地存储