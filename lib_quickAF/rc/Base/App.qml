import QtQuick 2.12
import "../Item"
import "qrc:/AFquick/Item"
import QtQuick.Layouts 1.3

Item {
    id: mainWindow
    anchors.fill: parent

    Rectangle {
        id: content_item
        color: "blue"
        objectName: "contentItem"
        anchors.top: AFapp.portrait ? title_bar.bottom : parent.top
        anchors.left: AFapp.portrait ? parent.left : menu_button.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Rectangle {
        id: preferences_item
        z: 1

        anchors.left: parent.left
        anchors.top: menu_button.bottom
        anchors.bottom: preferences_bar.top
        width: menu_bar.fullMode | open_sub_item ? AFapp.portrait ? parent.width : AFsize.menuWidth : AFsize.button
        visible: !AFapp.portrait | menu_bar.fullMode | open_sub_item
        opacity: !AFapp.portrait & menu_bar.fullMode ? 0.75 : 1
        color: "#7fffd4"

        property bool open_sub_item: false
        onOpen_sub_itemChanged: if (!open_sub_item) preferences_stack.currentIndex = 0

        function openPreference(index){
            open_sub_item = true
            preferences_stack.currentIndex = index
        }

        StackLayout {
            id: preferences_stack
            anchors.fill: parent

            Menu { // 0
                id: menu_bar
                z: 2
                color: "yellow"
            }

            Rectangle { // 1
                id: license_item
                color: "#03f884"
            }

            Rectangle { // 2
                id: account_item
                color: "#33f824"
            }

            Rectangle { // 3
                id: about_us_item
                color: "#7318f4"
            }

            Rectangle { // 4
                id: feed_back_item
                color: "#a348f4"
            }

            Rectangle { // 5
                id: settings_item
                color: "#3348f4"
            }
        }
    }

    Rectangle {
        id: title_bar
        anchors.top: parent.top
        anchors.left: menu_button.right
        anchors.right: (menu_bar.fullMode | preferences_item.open_sub_item) & !AFapp.portrait ? preferences_item.right : parent.right
        height: AFsize.button
        opacity: menu_bar.opacity
        color: "khaki"
        visible: AFapp.portrait | menu_bar.fullMode | preferences_item.open_sub_item
        z: 2
    }

    Rectangle {
        id: preferences_bar

        property bool pressMode: false
        z: 2
        color: "orange"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: AFsize.button
        width: AFapp.portrait ? parent.width : pressMode | menu_bar.fullMode ? AFsize.menuWidth : AFsize.button
        visible: AFapp.portrait ? menu_bar.fullMode : true

        AFbutton {
            id: p_open_preference_button
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: AFsize.button
            source: "qrc:/icon/material_design/show_more_button_with_three_dots.svg"
            visible: !AFapp.portrait & !menu_bar.fullMode & !preferences_item.open_sub_item

            onClicked: preferences_bar.pressMode = !preferences_bar.pressMode
        }

        RowLayout {
            anchors.left: AFapp.portrait | menu_bar.fullMode ? parent.left : p_open_preference_button.right
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            visible: preferences_bar.pressMode | menu_bar.fullMode
            spacing: 1

            Item {
                Layout.fillWidth: true
            }

            AFbutton {
                width: AFsize.button
                Layout.fillHeight: true
                source: "qrc:/icon/material_design/settings_cogwheel_button.svg"
                onClicked: preferences_item.openPreference(5)
            }
            AFbutton {
                width: AFsize.button
                Layout.fillHeight: true
                source: "qrc:/icon/material_design/user_shape.svg"
                onClicked: preferences_item.openPreference(2)
                visible: AFapp.account
            }
            AFbutton {
                width: AFsize.button
                Layout.fillHeight: true
                source: "qrc:/icon/material_design/underline_text_button.svg"
                onClicked: preferences_item.openPreference(3)
            }
            AFbutton {
                width: AFsize.button
                Layout.fillHeight: true
                source: "qrc:/icon/material_design/warning_sign.svg"
                onClicked: preferences_item.openPreference(4)
            }
            AFbutton {
                width: AFsize.button
                Layout.fillHeight: true
                source: "qrc:/icon/material_design/add_comment_button.svg"
                onClicked: preferences_item.openPreference(1)
            }
        }
    }

    AFbutton {
        id: menu_button
        z: 3
        anchors.top: parent.top
        anchors.left: parent.left
        height: AFsize.button
        width: AFsize.button
        source: preferences_item.open_sub_item
            ? "qrc:/icon/material_design/go_back_left_arrow.svg"
            : "qrc:/icon/material_design/menu_button.svg"

        onClicked: {
            if (preferences_item.open_sub_item){
                preferences_item.open_sub_item = false
                preferences_bar.pressMode = menu_bar.fullMode
            }
            else {
                menu_bar.fullMode = !menu_bar.fullMode
                preferences_bar.pressMode = false
            }
        }
    }
}
