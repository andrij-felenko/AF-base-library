import QtQuick 2.11
import qscolors 1.0

// TODO rewrite
Text {
    property url link: ""

    height: parent.height
    color: S1Colors.fieldBackground
//    font.underline: true
    font.pointSize: 10
    verticalAlignment: Text.AlignVCenter

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: Qt.openUrlExternally(link)
    }
}
