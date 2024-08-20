#include "QQMain.h"

void QQMainProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_Frame)
    {
        return;
    }
    else if (element == PE_FrameGroupBox)
    {
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQMainProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_PushButton)
    {
        QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
        if (buttonOption && (buttonOption->state & State_MouseOver))
        {
            buttonOption->state &= ~State_MouseOver;
            buttonOption->features &= ~QStyleOptionButton::Flat;
        }
        if (buttonOption && (buttonOption->state & State_Sunken))
        {
            buttonOption->state &= ~State_MouseOver;
            buttonOption->state &= ~State_Sunken;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QQMain::QQMain(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QQMainClass())
{
    ui->setupUi(this);
    m_buttonGroup = new QButtonGroup(this);
    m_proxyStyle = new QQMainProxyStyle;
    this->setStyle(m_proxyStyle);
    ui->group->setStyle(m_proxyStyle);
    ui->otherGroup->setStyle(m_proxyStyle);
    ui->setBtn->setStyle(m_proxyStyle);
    ui->infoBtn->setStyle(m_proxyStyle);
    QList<QAbstractButton *> buttons = ui->group->findChildren<QAbstractButton *>();
    for (int i = 0; i < buttons.count(); ++i)
    {
        m_buttonGroup->addButton(buttons.at(i), i);
    }

    ui->optionTab->tabBar()->setHidden(true);
    ui->optionTab->addTab(new QQChat, "");
    ui->optionTab->addTab(new QQFriend, "");

    connect(m_buttonGroup, &QButtonGroup::idClicked, ui->optionTab, &QTabWidget::setCurrentIndex);
}

QQMain::~QQMain()
{
    delete m_proxyStyle;
    delete ui;
}