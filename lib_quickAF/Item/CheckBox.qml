import QtQuick 2.9
import QtQuick.Controls 2.4
import qscolors 1.0

// TODO rewrite
CheckBox {
    id: m_checkBox
    anchors.centerIn: parent
    height: 16
    width: height

    property url check_icon: "qrc:/view/images/check mark.svg"
    property url uncheck_icon: "qrc:/view/images/check box.svg"
    property alias image: m_image


    indicator: Image {
        id: m_image
        anchors.fill: parent
        source: m_checkBox.checked ? check_icon : uncheck_icon
        sourceSize.width: width
        sourceSize.height: height
        clip: true
    }

    background: Rectangle {
        color: S1Colors.fieldBackground
        radius: 3
    }
}
