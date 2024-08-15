#ifndef QQ_COMMON_DEFINE_H
#define QQ_COMMON_DEFINE_H

#include <qglobal.h>
#include <qmutex.h>
#include <qevent.h>

template <class T>
class QMSingleton
{
public:
	static T *getInstance();

private:
	Q_DISABLE_COPY_MOVE(QMSingleton)
};

template <class T>
T *QMSingleton<T>::getInstance()
{
	static QMutex mutex;
	QMutexLocker lock(&mutex);
	static T *m_instance = nullptr;
	if (m_instance == nullptr)
	{
		m_instance = new T();
	}
	return m_instance;
}

// 类转化单例宏
#define Q_MSINGLETON_CREATE(Class)                \
private:                                          \
	friend class QMSingleton<Class>;              \
                                                  \
public:                                           \
	static Class *getInstance()                   \
	{                                             \
		return QMSingleton<Class>::getInstance(); \
	}

// 本地化类中枚举宏
#define Q_MDECLARE_FLAGS(Class, Enum)                                 \
	typedef struct Class : public Class{                              \
							   Q_DECLARE_FLAGS(Enum##s, Enum)} Class; \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Class::Enum##s)

#define Q_MDECLARE_FLAGS_BY_NAMESPACE(Class, Enum, Base, Config)      \
	typedef struct Class : public Base::Class{                        \
							   Q_DECLARE_FLAGS(Enum##s, Enum)} Class; \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Config::Class::Enum##s)

// 添加强类型枚举的取反，取同，异或（Qt5不支持,Qt6支持）
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_MDECLARE_OPERATORS_FOR_FLAGS(Flags) \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)
#else
#define Q_MDECLARE_OPERATORS_FOR_FLAGS(Flags)                                                                             \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)                                                                                  \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator&(Flags::enum_type f1, Flags::enum_type f2) noexcept         \
	{                                                                                                                     \
		return QFlags<Flags::enum_type>(f1) & f2;                                                                         \
	}                                                                                                                     \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator&(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept \
	{                                                                                                                     \
		return f2 & f1;                                                                                                   \
	}                                                                                                                     \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator^(Flags::enum_type f1, Flags::enum_type f2) noexcept         \
	{                                                                                                                     \
		return QFlags<Flags::enum_type>(f1) ^ f2;                                                                         \
	}                                                                                                                     \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator^(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept \
	{                                                                                                                     \
		return f2 ^ f1;                                                                                                   \
	}                                                                                                                     \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator~(Flags::enum_type f1) noexcept                              \
	{                                                                                                                     \
		return ~static_cast<int>(f1);                                                                                     \
	}
#endif

// 快速创建属性宏
#define Q_MPROPERTY_CREATE(Type, FunctionName, Name)                     \
	Q_PROPERTY(Type Name READ get##FunctionName WRITE set##FunctionName) \
public:                                                                  \
	void set##FunctionName(Type Name)                                    \
	{                                                                    \
		this->Name = Name;                                               \
	}                                                                    \
	Type get##FunctionName() const                                       \
	{                                                                    \
		return Name;                                                     \
	}                                                                    \
                                                                         \
private:                                                                 \
	Type Name;

// 快速创建事件宏
#define Q_MREGISTER_EVENT(EventName, Number)                                                    \
	class EventName : public QEvent                                                             \
	{                                                                                           \
	public:                                                                                     \
		static const QEvent::Type EventType = static_cast<QEvent::Type>(QEvent::User + Number); \
		explicit EventName() : QEvent(EventType) {}                                             \
		explicit EventName(const QByteArray &data) : QEvent(EventType), data(data) {}           \
		QByteArray data;                                                                        \
	};
// 有可能有缺陷(可能会导致界面卡顿)
#define Q_MSEND_EVENT(EventName, Data) \
	QCoreApplication::postEvent(QCoreApplication::instance(), new EventName(Data));
#define Q_MSEND_EVENT_BY_POINT(EventPoint) \
	QCoreApplication::postEvent(QCoreApplication::instance(), EventPoint);
#define Q_MSEND_EVENT_SINGLETON(EventName) \
	EventName *_event = new EventName;     \
	QCoreApplication::postEvent(QCoreApplication::instance(), _event);
#define Q_MHANDLE_EVENT(EventName, Handler)                  \
	case EventName::EventType:                               \
	{                                                        \
		EventName *_event = static_cast<EventName *>(event); \
		Handler(_event->data);                               \
		return true;                                         \
	}
#define Q_MHANDLE_EVENT_BY_RECEIVER(EventName, Receiver, Handler) \
	case EventName::EventType:                                    \
	{                                                             \
		EventName *_event = static_cast<EventName *>(event);      \
		Receiver->Handler(_event->data);                          \
		return true;                                              \
	}

#endif // QQ_COMMON_DEFINE_H