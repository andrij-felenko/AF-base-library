#ifndef LIB_QUICK_AF_GUI_SIZE_H
#define LIB_QUICK_AF_GUI_SIZE_H

#include <QtCore/QPoint>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

namespace AFquick {
    class GuiSize;

    typedef QSharedPointer <GuiSize> GuiSizePtr;

    GuiSizePtr guiSize();
}

class AFquick::GuiSize final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double button  READ button  CONSTANT)
    Q_PROPERTY(QPoint topleft READ topleft WRITE setTopleft NOTIFY topleftChanged)
    Q_PROPERTY(uint menuWidth READ menuWidth WRITE setMenuWidth NOTIFY menuWidthChanged)
public:
    explicit GuiSize(QObject* parent = nullptr);

    virtual double button()  const;
    virtual QPoint topleft() const;
    virtual uint menuWidth() const;

public slots:
    void setTopleft(QPoint topleft);
    void setMenuWidth(uint menuWidth);

signals:
    void topleftChanged(QPoint topleft);
    void menuWidthChanged(uint menuWidth);

private:
    double m_button;
    QPoint m_topleft;
    uint m_menuWidth;
};

typedef AFquick::GuiSize    AFguiSize;
typedef AFquick::GuiSizePtr AFguiSizePtr;

AFguiSizePtr afGuiSize();

#endif
