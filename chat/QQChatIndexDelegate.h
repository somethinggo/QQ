#ifndef QQ_CHAT_INDEXDELEGATE_H
#define QQ_CHAT_INDEXDELEGATE_H

#include <qstyleditemdelegate.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qicon.h>
#include <qevent.h>
#include <qstyleditemdelegate.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qfileinfo.h>

#include "common/qqfunction.h"

class QQChatIndexDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    QQ_PROPERTY_CREATE(QSize, ItemSize)
public:
    explicit QQChatIndexDelegate(QObject *parent = nullptr);
    ~QQChatIndexDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int8_t m_commonSpace;
signals:
    void clicked(const QModelIndex &index);
};

#endif // QQ_CHAT_INDEXDELEGATE_H