import QtQuick 2.12

Item {
    id: mainWindow
    anchors.fill: parent

    property string menuColor: "#4500ff"
    property string textColor: "white"
    property string darkMenuColor: "#0000d0"
    property int buttonSize: height / 11

    Rectangle {
        id: topRect
        objectName: "topRect"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: buttonSize
        color: menuColor

        Text {
            id: topRectText
            anchors.centerIn: parent
            font.pointSize: parent.height / 4
            color: textColor
        }
    }

    Rectangle {
        anchors.top: topRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Item {
            id: contentRect
            objectName: "contentItem"
            anchors.fill: parent
        }
    }
}
