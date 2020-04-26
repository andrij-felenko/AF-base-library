import QtQuick 2.12

Rectangle {
    property bool fullMode: false
    color: "green"

    ListView {
        anchors.fill: parent
        model: AFapp.menuModel

        delegate: Rectangle {
            width: parent.width
            height: AFsize.button
            color: model.is_current ? "yellow" : "orange"

            Text {
                anchors.centerIn: parent
                visible: fullMode

                text: model.name + " : " + model.icon_path
                font.pixelSize: parent.height * 0.6
            }
        }
    }
}
