#include "QQStorage.h"

QQStorage::QQStorage(QObject *parent)
    : QObject(parent)
{
    init();
}

QQStorage::~QQStorage()
{
}

void QQStorage::init()
{
    QString appPath = QCoreApplication::applicationDirPath();
}

bool QQStorage::event(QEvent *event)
{
    return QObject::event(event);
}

template <typename T>
std::string QQStorage::serializeToBase64(const T &message)
{
    // std::ostringstream oss;
    // boost::archive::text_oarchive oa(oss);
    // oa << message;
    // std::string data = oss.str();
    // typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> base64_enc;
    // std::string base64(base64_enc(data.begin()), base64_enc(data.end()));
    // return base64;
    return "";
}

template <typename T>
T QQStorage::deserializeFromBase64(const std::string &base64)
{
    // typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> base64_dec;
    // std::string data(base64_dec(base64.begin()), base64_dec(base64.end()));
    // std::istringstream iss(data);
    // boost::archive::text_iarchive ia(iss);
    // T message;
    // ia >> message;
    // return message;
    return T();
}

template <typename T>
T *QQStorage::deserializeFromBase64(const std::string &base64)
{
    // typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> base64_dec;
    // std::string data(base64_dec(base64.begin()), base64_dec(base64.end()));
    // std::istringstream iss(data);
    // boost::archive::text_iarchive ia(iss);
    // T *message = new T();
    // ia >> *message;
    // return message;
    return nullptr;
}