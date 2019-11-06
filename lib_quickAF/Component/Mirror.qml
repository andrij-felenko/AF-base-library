import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.platform 1.0

// TODO rewrite
Item {
    property alias checkItem: checkItem_.children
    property alias rightItem: rightItem_.children
    property alias rightWidth: rightItem_.width
    property alias title: text_.text
    property alias preTitle: text_.preText
    property alias sufTitle: text_.sufText
    property alias textItem: text_

    height: 22

    TrayText {
        id: text_
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        fontSize: height * 0.55
    }
    
    Item {
        id: checkItem_
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: rightItem_.left
        anchors.rightMargin: 5

        width: height
    }

    Item {
        id: rightItem_
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: Math.floor(parent.width / 2)
    }
}
