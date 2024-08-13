#ifndef QQ_CHAT_MESSAGEDELEGATE_H
#define QQ_CHAT_MESSAGEDELEGATE_H

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

class QQChatMessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QQChatMessageDelegate(QObject *parent = nullptr);
    virtual ~QQChatMessageDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const qreal m_iconMargin = 25;
    const qreal m_contentMargin = 75;
    const qreal m_contentPadding = 10;
    const qreal m_nameMargin = 5;
    QSize m_iconSize = QSize(40, 40);
};

#endif // QQ_CHAT_MESSAGEDELEGATE_H