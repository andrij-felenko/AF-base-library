import QtQuick 2.9
import QtQuick.Controls 2.2
import qscolors 1.0

// TODO rewrite
Rectangle {
    anchors.top: parent.top
    anchors.topMargin: 1
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 1
    clip: true
    radius: 3

    property alias input: m_input.children

    border.color: S1Colors.fieldBorder
    border.width: 1

    signal currentChanged(string text)
    signal currentTextChanged(string text)

    Item {
        id: m_input
        anchors.fill: parent
        anchors.margins: 2
        clip: true
    }
}
