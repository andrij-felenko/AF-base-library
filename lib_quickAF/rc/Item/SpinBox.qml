import QtQuick 2.9
import QtQuick.Controls 2.2
import qscolors 1.0

// TODO rewrite
SpinBox {
    id: m_spinBox
    clip: true
    font.pixelSize: height / 2
    anchors.fill: parent
    implicitHeight: height
    implicitWidth: width

    background: Rectangle {
        color: S1Colors.fieldBackground
        radius: 3
        border.width: 1
        border.color: S1Colors.fieldBorder
    }

    down.indicator: Item {
        id: m_down
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: height

        Image {
            anchors.fill: parent
            anchors.margins: parent.height * 0.3
            sourceSize.width: width
            sourceSize.height: height
            source: Qt.resolvedUrl(m_down.enabled ? "qrc:/view/images/minus button.svg"
                                                  : "qrc:/view/images/minus_d.svg")
        }
    }

    up.indicator: Item {
        id: m_up
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: height

        Image {
            anchors.fill: parent
            anchors.margins: parent.height * 0.3
            sourceSize.width: width
            sourceSize.height: height
            source: Qt.resolvedUrl(m_up.enabled ? "qrc:/view/images/plus button.svg"
                                                : "qrc:/view/images/plus_d.svg")
        }
    }
}
