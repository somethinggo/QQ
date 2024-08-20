#ifndef QQ_MAIN_H
#define QQ_MAIN_H

#include <qmainwindow.h>
#include <qproxystyle.h>
#include <qbuttongroup.h>

#include "ui_QQMain.h"
#include "chat/QQChat.h"
#include "friend/QQFriend.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class QQMainClass;
};
QT_END_NAMESPACE

class QQMainProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit QQMain(QWidget *parent = nullptr);
    ~QQMain();

private:
    Ui::QQMainClass *ui;
    QProxyStyle *m_proxyStyle;
    QButtonGroup *m_buttonGroup;
};

#endif // QQ_MAIN_H