#pragma once

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

#include "qfunction.hpp"

class QQChatMessageSingleViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QQChatMessageSingleViewDelegate(QObject *parent = nullptr);
    virtual ~QQChatMessageSingleViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    const qreal m_iconMargin = 25;
    const qreal m_contentMargin = 75;
    const qreal m_contentPadding = 10;
    const qreal m_nameMargin = 5;
    QSize m_iconSize;
};

class QQSinglePeopleWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_MPROPERTY_CREATE(QSize, MItemSize, m_itemSize)
    Q_MPROPERTY_CREATE(qreal, MMAXStringWidth, m_maxStringWidth)
public:
    explicit QQSinglePeopleWidgetDelegate(QObject *parent = nullptr);
    virtual ~QQSinglePeopleWidgetDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QSize m_iconSize;
    int8_t m_commonSpace;
signals:
    void clicked(const QModelIndex &index);
};

class QQSingleFriendWidgetDelegate : public QQSinglePeopleWidgetDelegate
{
    Q_OBJECT
public:
    explicit QQSingleFriendWidgetDelegate(QObject *parent = nullptr);
    virtual ~QQSingleFriendWidgetDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QSize m_iconSize;
    int8_t m_commonSpace;
};