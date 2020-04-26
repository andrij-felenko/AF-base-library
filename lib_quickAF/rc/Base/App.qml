import QtQuick 2.12

Item {
    id: mainWindow
    anchors.fill: parent

    // TODO it must be button wth svg icon
    Rectangle { // NOTE menu buttton
        id: menu_button
        anchors.top: parent.top
        anchors.left: parent.left
        height: AFsize.button
        width: AFsize.button
        color: "blue"
        z: 2

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("click")
                console.log(menu_bar.fullMode)
                menu_bar.fullMode = !menu_bar.fullMode
                console.log(menu_bar.fullMode, menu_bar.width, menu_bar.visible)
            }
        }
    }

    Rectangle { // NOTE title item, only with portrain mode
        id: title_bar
        anchors.top: parent.top
        anchors.left: menu_button.right
        anchors.right: menu_bar.fullMode & !AFapp.portrait ? menu_bar.right : parent.right
        height: AFsize.button
        opacity: menu_bar.opacity
        color: "khaki"
        visible: AFapp.portrait || menu_bar.fullMode
        z: 1
    }

    Menu { // NOTE left menu, have short mode and full mode
        id: menu_bar
        anchors.left: parent.left
        anchors.top: menu_button.bottom
        anchors.bottom: parent.bottom
        width: fullMode ? AFapp.portrait ? parent.width : 300 : AFsize.button
        visible: !AFapp.portrait || fullMode
        opacity: !AFapp.portrait & fullMode ? 0.85 : 1
        z: 1
    }

    Item {
        id: content_item
        objectName: "contentItem"
        anchors.top: AFapp.portrait ? title_bar.bottom : parent.top
        anchors.left: AFapp.portrait ? parent.left : menu_button.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
