#ifndef QQ_FRIEND_INDEXDELEGATE_H
#define QQ_FRIEND_INDEXDELEGATE_H

#include <qwidget.h>
#include <qlabel.h>
#include <qicon.h>
#include <qevent.h>
#include <qstyleditemdelegate.h>
#include <qpainter.h>
#include <qpainterpath.h>

#include "common/qqfunction.h"
#include "common/qqglobal.h"

class QQFriendIndexDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_MPROPERTY_CREATE(QSize, ItemSize, m_itemSize)
    Q_MPROPERTY_CREATE(qreal, MAXStringWidth, m_maxStringWidth)
public:
    explicit QQFriendIndexDelegate(QObject *parent = nullptr);
    virtual ~QQFriendIndexDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    int8_t m_commonSpace;
};

#endif // QQ_FRIEND_INDEXDELEGATE_H