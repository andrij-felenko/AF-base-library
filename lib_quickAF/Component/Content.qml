import QtQuick 2.0

// TODO rewrite
Item {
    property alias header: headerItem.children
    property alias content: contentItem.children
    readonly property real separateY: headerSeparate.y + spacing

    Item {
        id: headerItem
        anchors.top: parent.top

        width: parent.width
        height: 55
    }

    SeparateItem {
        id: headerSeparate
        anchors.top: headerItem.bottom
        anchors.topMargin: spacing

        width: parent.width
    }

    Item {
        id: contentItem
        anchors.top: headerSeparate.bottom
        anchors.topMargin: spacing
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: spacing
    }
}
