#ifndef QQ_COMMON_DEFINE_H
#define QQ_COMMON_DEFINE_H

#include <qglobal.h>
#include <qevent.h>

/**
 * @brief 通用宏定义
 * @note 未私有化类中的构造函数,析构函数
 */
#define QQ_SINGLETON_CREATE(Class) \
	Q_DISABLE_COPY_MOVE(Class)     \
public:                            \
	static Class *getInstance()    \
	{                              \
		static Class instance;     \
		return &instance;          \
	}

/**
 * @brief 设置强类型枚举的位运算
 */
#define QQ_DECLARE_FLAGS(Class, Enum)     \
	Q_DECLARE_FLAGS(Enum##s, Class::Enum) \
	QQ_DECLARE_OPERATORS_FOR_FLAGS(Enum##s)

#define Q_MDECLARE_FLAGS_BY_NAMESPACE(Class, Enum, Base, Config)      \
	typedef struct Class : public Base::Class{                        \
							   Q_DECLARE_FLAGS(Enum##s, Enum)} Class; \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Config::Class::Enum##s)

/**
 * @brief 添加强类型枚举的取反，取同，异或
 * @note Qt5不支持,Qt6支持
 */
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define QQ_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)
#else
#define QQ_DECLARE_OPERATORS_FOR_FLAGS(Flags)                                                                                                                   \
	Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)                                                                                                                        \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator&(Flags::enum_type f1, Flags::enum_type f2) noexcept { return QFlags<Flags::enum_type>(f1) & f2; } \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator&(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept { return f2 & f1; }                   \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator^(Flags::enum_type f1, Flags::enum_type f2) noexcept { return QFlags<Flags::enum_type>(f1) ^ f2; } \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator^(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept { return f2 ^ f1; }                   \
	Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator~(Flags::enum_type f1) noexcept { return QFlags<Flags::enum_type>(~static_cast<int>(f1)); }
#endif

/**
 * @brief 创建普通属性宏
 */
#define QQ_PROPERTY_CREATE(Type, Name)                   \
	Q_PROPERTY(Type Name READ get##Name WRITE set##Name) \
public:                                                  \
	void set##Name(Type Name)                            \
	{                                                    \
		this->m_##Name = Name;                           \
	}                                                    \
	Type get##Name() const                               \
	{                                                    \
		return m_##Name;                                 \
	}                                                    \
                                                         \
private:                                                 \
	Type m_##Name;

/**
 * @brief 注册元类型宏
 */
#define QQ_REGISTER_METATYPE(Type) \
	Q_DECLARE_METATYPE(Type);

/**
 * @brief 注册事件宏
 */
#define QQ_REGISTER_EVENT(EventName, Number)                                                    \
	class EventName : public QEvent                                                             \
	{                                                                                           \
	public:                                                                                     \
		static const QEvent::Type EventType = static_cast<QEvent::Type>(QEvent::User + Number); \
		explicit EventName() : QEvent(EventType) {}                                             \
		explicit EventName(const QByteArray &data) : QEvent(EventType), data(data) {}           \
		QByteArray data;                                                                        \
	};

/**
 * @brief 发送事件宏
 * @note 有可能有缺陷(可能会导致界面卡顿)
 */
#define QQ_SEND_EVENT(EventName, Data) \
	QCoreApplication::postEvent(QCoreApplication::instance(), new EventName(Data));
#define QQ_SEND_EVENT_NODATA(EventName) \
	QQ_SEND_EVENT(EventName, QByteArray());

/**
 * @brief 处理事件宏
 */
#define QQ_HANDLE_EVENT(EventName, Receiver, Handler)             \
	case EventName::EventType:                                    \
	{                                                             \
		Receiver->Handler(static_cast<EventName *>(event)->data); \
		return true;                                              \
	}
#define QQ_HANDLE_EVENT_THIS(EventName, Handler) \
	QQ_HANDLE_EVENT(EventName, this, Handler);

#endif // QQ_COMMON_DEFINE_H