#pragma once

#include <qglobal.h>
#include <qmutex.h>

#include "qglobalvalue.h"

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
		GlobalValuesAgent::m_singletons.append(m_instance);
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

// 类释放单例列表宏
#define Q_MSINGLETON_FREE                                            \
	for (int i = 0; i < GlobalValuesAgent::m_singletons.size(); ++i) \
	{                                                                \
		delete GlobalValuesAgent::m_singletons[i];                   \
		GlobalValuesAgent::m_singletons[i] = nullptr;                \
	}

// 本地化类中枚举宏
#define Q_MDECLARE_FLAGS(Class, Enum)                                 \
	typedef struct Class : public BaseConfigs::Class{                 \
							   Q_DECLARE_FLAGS(Enum##s, Enum)} Class; \
	Q_DECLARE_OPERATORS_FOR_FLAGS(ClientConfigs::Class::Enum##s)

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