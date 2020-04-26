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
public:
    explicit GuiSize(QObject* parent = nullptr);

    virtual double button()  const;
    virtual QPoint topleft() const;

public slots:
    void setTopleft(QPoint topleft);

signals:
    void topleftChanged(QPoint topleft);

private:
    double m_button;
    QPoint m_topleft;
};

typedef AFquick::GuiSize    AFguiSize;
typedef AFquick::GuiSizePtr AFguiSizePtr;

AFguiSizePtr afGuiSize();

#endif
