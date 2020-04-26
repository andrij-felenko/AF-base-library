import QtQuick 2.11
import "../Object"

Item {
    id: m_button

    signal clicked()

    property alias text: m_text.text
    property alias border: m_rectangle.border
    property alias radius: m_rectangle.radius
    property alias fontSize: m_text.font.pixelSize
    property alias horizontalAlignment: m_text.horizontalAlignment

    property bool hoverSensitive: true
    property bool preferableIconTop:  true
    property bool preferableIconLeft: true
    property real scalabilityIcon: 0.7

    // source
    property alias source: m_source
    UrlButtonObject { id: m_source }

    // rectangle
    property alias color: m_rect_color
    ColorButtonObject { id: m_rect_color }

    // text
    property alias colorText: m_text_color
    ColorButtonObject { id: m_text_color; enable: "black" }

    height: 32
    radius:  4

    Item {
        id: _p
        property bool isHover: false
        property bool isPress: false
        property bool square: m_button.height / m_button.width < 3 && m_button.width / m_button.height < 3
        property bool horizontal: m_button.width / m_button.height >= 3
        property bool vertical: m_button.height / m_button.width >= 3
    }

    Rectangle {
        id: m_rectangle
        anchors.fill: parent
        clip: true

        color: _p.isPress ? m_rect_color.click
                          : enabled ? _p.isHover ? m_rect_color.hover : m_rect_color.enable
                                    : m_rect_color.disable
        border.width: 1
        border.color: Qt.darker(m_rect_color.enable)

        Item {
            id: m_textItem
            anchors {
                top: preferableIconTop && m_imageItem.visible ? m_imageItem.bottom : parent.top
                left: _p.horizontal && !preferableIconLeft && m_imageItem.visible ? m_imageItem.right : parent.left
                right: _p.horizontal && preferableIconLeft && m_imageItem.visible ? m_imageItem.left : parent.right
                bottom: !preferableIconTop && m_imageItem.visible ? m_imageItem.top : parent.bottom
            }
            height: _p.square ? Math.min(m_button.height, m_button.width) / 4 : m_button.height
            visible: m_text.text !== "" && (m_button.width / m_button.height > 3
                                            || scalabilityIcon < 0.75
                                            || m_button.height / m_button.width > scalabilityIcon + 0.25)


            Text {
                id: m_text
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: _p.isPress ? m_text_color.click
                                  : enabled ? _p.isHover ? m_text_color.hover : m_text_color.enable
                                            : m_text_color.disable
                rotation: _p.vertical ? preferableIconLeft ? 270 : 90 : 0
            }
        }

        Item {
            id: m_imageItem
            anchors {
                centerIn: m_textItem.visible ? undefined : parent
                left:  !m_textItem.visible && !preferableIconLeft ? unedfined : parent.left
                right: !m_textItem.visible &&  preferableIconLeft ? undefined : parent.right
                top:    m_textItem.visible &&  preferableIconTop ? parent.top    : undefined
                bottom: m_textItem.visible && !preferableIconTop ? parent.bottom : undefined
                leftMargin:  !_p.square || m_imageItem.height >= m_button.width ? 0 : (m_button.width - m_imageItem.width) / 2
                rightMargin: !_p.square || m_imageItem.height >= m_button.width ? 0 : (m_button.width - m_imageItem.width) / 2
            }
            height: _p.vertical ? m_button.width
                                : _p.horizontal || !m_textItem.visible ? Math.min(m_button.height, m_button.width)
                                                                   : Math.min(m_button.width, 0.75 * m_button.height)
            width: m_imageItem.height
            visible: m_image.source != ""

            Image {
                id: m_image
                anchors.centerIn: parent
                width: m_imageItem.width * scalabilityIcon
                height: m_imageItem.height * scalabilityIcon
                sourceSize.width: m_image.width
                sourceSize.height: m_image.height
                source: _p.isPress ? m_source.click
                                   : enabled ? _p.isHover ? m_source.hover : m_source.enable
                                             : m_source.disable
            }
        }

        MouseArea {
            id: m_mousearea
            anchors.fill: parent
            hoverEnabled: true
            onPressed:  _p.isPress = true
            onReleased: _p.isPress = false
            onClicked: /* emit */ m_button.clicked();
            onEntered: if (hoverSensitive) _p.isHover = true
            onExited:  if (hoverSensitive) _p.isHover = false
        }
    }
}
