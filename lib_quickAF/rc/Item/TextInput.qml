import QtQuick 2.11

// TODO rewrite
MirrorItem {
    anchors.left: parent.left
    anchors.right: parent.right

    property alias textInput: p_text_input_item
    property alias inputRectangle: textInput
    title: qsTr("Server host")

    rightItem: TextInputRectangle {
        id: textInput
        anchors.fill: parent

        input: TextInput {
            id: p_text_input_item
            anchors.fill: parent
        }
    }
}
