#include "PushButton.h"
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <QSizePolicy>
#include <QSpacerItem>


PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{
    initUILayout();
}

void PushButton::setButtonStyleSheet()
{
    this->setObjectName("pButtonOk");

    QString styleSheetQSS = "";

    //如果启动悬浮模式则使用悬浮qss
    if(m_hoverEnabled)
    {
        styleSheetQSS = QString("QPushButton{outline:0px;border:0px;}" \
                                "QPushButton#pButtonOk  \
                                {   background-color:%1; \
                                    border: %4 solid %5; \
                                } \
                                QPushButton#pButtonOk:hover \
                                { \
                                    color:%2;   \
                                    background-color:%3; \
                                    border: %4 solid %5; \
                                } \
                                ").arg(m_backgroundColor, m_hoverFontColor, m_hoverBgColor, m_borderWide, m_borderColor);
    }
    else
    {
        qInfo()<<"进来----";
        styleSheetQSS = QString("QPushButton#pButtonOk  \
        { \
                                    background-color:%1; \
                                    border-radius:2px;  \
                                    border:%2 solid %3;}").arg(m_backgroundColor, m_borderWide, m_borderColor);
    }

    this->setStyleSheet(styleSheetQSS);

}

void PushButton::initUILayout()
{

    QSizePolicy  sizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);

    m_horizontalLayout = new QHBoxLayout(this);
     //加弹簧
    m_HspacerItemStart = new QSpacerItem(20,20, QSizePolicy::Expanding);
    m_horizontalLayout->addSpacerItem(m_HspacerItemStart);

//    horizontalLayout->setStretch(0, 1);
//    horizontalLayout->setStretch(1, 4);
    //加弹簧
    m_HspacerItemEnd = new QSpacerItem(20,20, QSizePolicy::Expanding);
    m_horizontalLayout->addSpacerItem(m_HspacerItemEnd);

    m_horizontalLayout->setContentsMargins(0, 0, 0,0);
    this->setText("");

}

void PushButton::setButtonMaxAndMinValue(QWidget *widget , QSize size)
{
    //设置最大值
    widget->setMaximumWidth(size.width());
    widget->setMaximumHeight(size.height());

    //设置最小值
    widget->setMinimumWidth(size.width());
    widget->setMinimumHeight(size.height());
}

void PushButton::setClickEffect(bool clickEffect)
{
    m_isClickEffect = clickEffect;
}

void PushButton::setBackgroundImage(QString imgPath, QSize size)
{
    m_backGroundImage = imgPath;
    this->setIcon(QIcon(QPixmap(m_backGroundImage)));
    this->setIconSize(size);
}

void PushButton::setButtonTransparency(bool isTransparency)
{
    this->setStyleSheet("QPushButton \
    { \
                            color:white; \
                            background-color:#00000000; \
                            border: 0px solid rgb(11 , 137 , 234); \
                        } "
                        );
}


void PushButton::mousePressEvent(QMouseEvent *e)
{
    if(m_isClickEffect){
        pressClickStyleSheet();
    }
    e->accept();
}

void PushButton::mouseReleaseEvent(QMouseEvent *e)
{
    sigClick();
    sigClick(m_isClickSecond);
    if(m_isClickEffect){
        releaseClickStyleSheet();
    }

    m_isClickSecond = !m_isClickSecond;

    e->accept();
}

void PushButton::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
}

void PushButton::pressClickStyleSheet()
{
    /*
    this->setStyleSheet(QString("#pButtonOk  \
    {  \
                                    color:%1;  \
                                    border: %3 solid %4;  \
                                    padding-left:3px; \
                                    padding-top:3px; \
                                }").arg(m_fontColor, m_borderWide, m_borderColor));
                                */

    m_horizontalLayout->setContentsMargins(4, 3, 0, 0);

}

void PushButton::releaseClickStyleSheet()
{

   m_horizontalLayout->setContentsMargins(0, 0, 0, 0);
    /*
    this->setStyleSheet(QString("#buttonText#button  \
    {  \
                                    color:%1;  \
                                    border: %3 solid %4;  \
                                    padding-left:0px; \
                                    padding-top:0px; \
                                }").arg(m_fontColor, m_borderWide, m_borderColor));
                                */

}

void PushButton::setBackgroundColor(QString backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    if(!backgroundColor.isEmpty())
    {
        m_backgroundColor = backgroundColor;
    }

    setButtonStyleSheet();
}

void PushButton::setFontColor(QString fontColor)
{
        m_fontColor = fontColor;
        m_buttonText->setStyleSheet(QString("#buttonText{color:%1}").arg(m_fontColor));
}

void PushButton::setButtonText(const QString &text)
{
     m_horizontalLayout->removeItem(m_HspacerItemEnd);
    //初始化ui
    if(m_buttonText == nullptr)
    {
        m_buttonText = new QLabel(this);
        m_buttonText->setObjectName("buttonText");
        m_buttonText->setText(this->text());
        this->setText("");
    }

    m_buttonText->setText(text);
    m_horizontalLayout->insertWidget(2, m_buttonText);
    m_horizontalLayout->addSpacerItem(m_HspacerItemEnd);
}

void PushButton::setText(const QString &text)
{
    setButtonText(text);
}

void PushButton::setLeftIcon(const QString &Licon, const QSize & size)
{
     m_horizontalLayout->removeItem(m_HspacerItemEnd);
    if(m_buttonLeftIcon == nullptr)
    {
        m_buttonLeftIcon = new QLabel(this);
        m_buttonLeftIcon->setObjectName("buttonLIcon");
        m_buttonLeftIcon->setScaledContents(true);
    }

    if(!size.isEmpty())
    {
        setButtonMaxAndMinValue(m_buttonLeftIcon, size);
    }
    m_leftIconSource = Licon;

    m_buttonLeftIcon->setStyleSheet("#buttonLIcon { background-position:center; background-repeat:no_repeat; image: url("+Licon+");}");

    m_horizontalLayout->insertWidget(1, m_buttonLeftIcon);
    m_horizontalLayout->addSpacerItem(m_HspacerItemEnd);
}

void PushButton::setRightIcon(const QString &Ricon, const QSize &size)
{
    m_horizontalLayout->removeItem(m_HspacerItemEnd);
    if(m_buttonRightIcon == nullptr)
    {
        m_buttonRightIcon = new QLabel(this);
        m_buttonRightIcon->setObjectName("buttonRIcon");
        m_buttonRightIcon->setScaledContents(true);
    }
    else
    {
        m_horizontalLayout->removeWidget(m_buttonRightIcon);
    }

    if(!size.isEmpty())
    {
         setButtonMaxAndMinValue(m_buttonRightIcon, size);
    }

    m_buttonRightIcon->setStyleSheet("");
    m_buttonRightIcon->setStyleSheet("#buttonRIcon {image: url("+Ricon+");}");

   m_buttonRightIcon->setStyle(QApplication::style());

    m_horizontalLayout->insertWidget(3, m_buttonRightIcon);
    m_horizontalLayout->addSpacerItem(m_HspacerItemEnd);
}

void PushButton::setHoverEnabled(bool Enabled)
{
    m_hoverEnabled = Enabled;
}

void PushButton::setHoverColor(const QString &hoverBgColor, const QString &hoverFontColor)
{
    m_hoverBgColor = hoverBgColor;
    m_hoverFontColor = hoverFontColor;
}

void PushButton::setButtonBorderWideAndColor(const QString &borderWide, const QString &borderColor)
{
    m_borderColor = borderColor;
    m_borderWide = borderWide;
}

void PushButton::enterEvent(QEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    event->accept();
}

void PushButton::leaveEvent(QEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    event->accept();
}
