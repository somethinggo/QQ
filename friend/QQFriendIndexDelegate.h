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

class QQFriendIndexDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    QQ_PROPERTY_CREATE(QSize, ItemSize)
public:
    explicit QQFriendIndexDelegate(QObject *parent = nullptr);
    ~QQFriendIndexDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    int8_t m_commonSpace;
};

#endif // QQ_FRIEND_INDEXDELEGATE_H