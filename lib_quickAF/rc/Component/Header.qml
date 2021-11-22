import QtQuick 2.11
import "../Item" as MQItem

Rectangle {
    id: m_headerItem
    property alias content: m_content.children
    property alias  leftButton:  m_leftIconButton
    property alias rightButton: m_rightIconButton
    color: "transparent"

    signal  leftButtonClick()
    signal rightButtonClick()

    MQItem.Button {
        id: m_leftIconButton
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        visible: false
        width: height
        onClicked: /* emit */ m_headerItem.leftButtonClick()
    }

    Item {
        id: m_content
        anchors {
            top: parent.top
            left: m_leftIconButton.visible ? m_leftIconButton.right : parent.left
            right: m_rightIconButton.visible ? m_rightIconButton.left : parent.right
            bottom: parent.bottom
        }
        Rectangle {
            color: "green"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height / 2
        }
    }

    MQItem.Button {
        id: m_rightIconButton
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        visible: false
        width: height
        onClicked: /* emit */ m_headerItem.rightButtonClick()
    }
}
