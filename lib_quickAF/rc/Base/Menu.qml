import QtQuick 2.12
import "../Item"
import "qrc:/AFquick/Item"

Rectangle {
    property bool fullMode: false
    color: "green"

    ListView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.fill: parent
        model: AFapp.menuModel

        delegate: Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: AFsize.button
            color: model.is_current ? "yellow" : "orange"

            Item {
                id: p_image
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: AFsize.button

                Image {
                    anchors.centerIn: parent
                    anchors.margins: AFsize.button * 0.1
                    sourceSize.width: parent.width * 0.8
                    sourceSize.height: parent.height * 0.8
                    source: Qt.resolvedUrl(model.icon_path)
                }
            }

            Text {
                anchors.left: p_image.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                visible: fullMode
                text: model.name
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.height * 0.5
            }
        }
    }
}
