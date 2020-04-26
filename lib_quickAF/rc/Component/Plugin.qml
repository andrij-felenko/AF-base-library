import QtQuick 2.11

Item {
    id: appBase
    property alias headerItem: m_headerItem.children
    property alias contentItem: m_contentItem.children

    Item {
        id: m_headerItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: _p.menuHeight
    }

    Item {
        id: m_contentItem
        anchors.top: m_headerItem.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Item {
        id: _p
        property real menuHeight: Math.min(appBase.height / 11, appBase.width / 11)
    }
}
