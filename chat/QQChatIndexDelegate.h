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

#include "qqglobal.h"
#include "qqfunction.h"

class QQChatIndexDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_MPROPERTY_CREATE(QSize, ItemSize, m_itemSize)
    Q_MPROPERTY_CREATE(qreal, MAXStringWidth, m_maxStringWidth)
public:
    explicit QQChatIndexDelegate(QObject *parent = nullptr);
    virtual ~QQChatIndexDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSize m_iconSize;
    int8_t m_commonSpace;
signals:
    void clicked(const QModelIndex &index);
};

#endif // QQ_CHAT_INDEXDELEGATE_H