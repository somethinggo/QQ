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
