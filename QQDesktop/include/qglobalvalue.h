#pragma once

#include <qqueue.h>
#include <qthread.h>
#include <qsharedpointer.h>
#include <qlocalsocket.h>
#include <qmutex.h>

#include "base.h"

namespace GlobalValuesAgent
{
	using namespace BaseConstValues;	   // 基础常量值
	const QString AUDIO_FORMAT = "amr";	   // 音频文件格式
	const int MAX_INPUT_TEXT_LENGTH = 500; // 最大输入文本长度

	extern QLocalSocket m_localSocket;		   // 本地socket
	extern BaseConfigs::UserBaseConfig m_user; // 当前登录的用户信息
	extern QList<QObject *> m_singletons;	   // 单例模式的管理器
	extern QQueue<QThread *> m_threadPool;	   // 线程的管理器
}
