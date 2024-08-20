#ifndef QQ_STORAGE_STORAGE_H
#define QQ_STORAGE_STORAGE_H

#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtemporaryfile.h>
#include <qstring.h>
#include <fstream>

#include <fstream>
#include <sstream>
#include <iostream>
// #include <boost/archive/text_oarchive.hpp>
// #include <boost/archive/text_iarchive.hpp>
// #include <boost/archive/iterators/base64_from_binary.hpp>
// #include <boost/archive/iterators/binary_from_base64.hpp>
// #include <boost/archive/iterators/transform_width.hpp>

#include "common/qqfunction.h"

class QQStorage : public QObject
{
    Q_OBJECT
    QQ_SINGLETON_CREATE(QQStorage)
public:
    explicit QQStorage(QObject *parent = nullptr);
    ~QQStorage();

    void init();

protected:
    bool event(QEvent *event) override;

private:
    /**
     * @brief 序列化对象为Base64字符串
     * @tparam T 对象类型
     * @param message 对象
     * @return std::string Base64字符串
     */
    template <typename T>
    std::string serializeToBase64(const T &message);

    /**
     * @brief 反序列化Base64字符串为对象
     * @tparam T 对象类型
     * @param base64 Base64字符串
     * @return T 对象
     */
    template <typename T>
    T deserializeFromBase64(const std::string &base64);

    template <typename T>
    T *deserializeFromBase64(const std::string &base64);
};

#endif // QQ_STORAGE_STORAGE_H