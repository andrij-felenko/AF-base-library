import QtQuick 2.11
import "../Object" as MQObject
import "../Enum"

Item {
    MQObject.TextObject {
        id: m_main;
        scalability: 1
    } property alias mainText: m_main

    MQObject.TextObject {
        id: m_pre
        color: m_main.color
        horizontal: Qt.AlignRight
    } property alias prefixText: m_pre

    MQObject.TextObject {
        id: m_suf
        color: m_main.color
        horizontal: Qt.AlignLeft
    } property alias sufixText: m_suf

    property real fontSize: height * 0.45

    Text {
        id: m_preItem
        visible: text !== ""
        text: m_pre.text
        font.pixelSize: fontSize * m_pre.scalability
        height: font.pixelSize
        width: implicitWidth
        horizontalAlignment: m_pre.horizontal
        verticalAlignment: m_pre.vertical
        anchors {
            bottom: m_mainItem.bottom
            left: parent.left
            rightMargin: 1
            right: m_mainItem.visible ? m_mainItem.left : m_sufItem.visible ? m_sufItem.left : parent.right
        }
    }

    Text {
        id: m_mainItem
        visible: m_main.text !== ""
        text: m_main.text
        font.pixelSize: fontSize
        width: implicitWidth
        height: font.pixelSize
        horizontalAlignment: m_main.horizontal
        verticalAlignment: m_main.vertical
        anchors.centerIn: parent
    }

    Text {
        id: m_sufItem
        visible: m_suf.text !== ""
        text: m_suf.text
        font.pixelSize: fontSize * m_suf.scalability
        height: font.pixelSize
        width: implicitWidth
        horizontalAlignment: m_suf.horizontal
        verticalAlignment: m_suf.vertical
        anchors {
            bottom: m_mainItem.bottom
            left: m_mainItem.visible ? m_mainItem.right : m_preItem.visible ? m_preItem.right : parent.left
            right: parent.right
            leftMargin: 1
        }
    }
}
