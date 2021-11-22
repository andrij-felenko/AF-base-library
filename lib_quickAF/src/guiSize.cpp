#include "guiSize.h"
#include <QtGui/QGuiApplication>
#include <AFbase/AfSystem>
#include <QtGui/QScreen>

AFguiSizePtr afGuiSizePtr;

AFquick::GuiSize::GuiSize(QObject *parent) : QObject(parent)
{
    if (AFlib::System::isMobile()){
        auto screenGeo = qApp->screens().first()->geometry();
        m_button = std::min(screenGeo.width(), screenGeo.height()) / 7;
    }
    else
        m_button = 40;
    m_menuWidth = 250;
}

double AFquick::GuiSize::button() const
{
    return m_button;
}

QPoint AFquick::GuiSize::topleft() const
{
    return m_topleft;
}

uint AFquick::GuiSize::menuWidth() const
{
    return m_menuWidth;
}

void AFquick::GuiSize::setTopleft(QPoint topleft)
{
    if (m_topleft == topleft)
        return;

    m_topleft = topleft;
    emit topleftChanged(m_topleft);
}

void AFquick::GuiSize::setMenuWidth(uint menuWidth)
{
    if (m_menuWidth == menuWidth)
        return;

    m_menuWidth = menuWidth;
    emit menuWidthChanged(m_menuWidth);
}

AFguiSizePtr afGuiSize()
{
    return AFquick::guiSize();
}

AFquick::GuiSizePtr AFquick::guiSize()
{
    if (afGuiSizePtr.isNull())
        afGuiSizePtr = AFguiSizePtr::create(qApp);

    return afGuiSizePtr;
}
