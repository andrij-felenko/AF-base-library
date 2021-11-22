import QtQuick 2.12

Rectangle {
    id: m_af_button

    property alias text: m_text.text
    property alias fontSize: m_text.font.pixelSize
    property alias hAlignment: m_text.horizontalAlignment

    property bool isHover: false
    property bool hoverSensitive: true

    property string source: ""
    property string sourceHover: ""
    property string colorHover: "yellow"

    property double imageCoef: 0.6

    height: AFsize.button
    radius: 3
    // TODO
    color: enabled ? isHover ? "#bbbbbb"
                             : "#aaaaaa"
                   : "blue"

    border.width: 1
    border.color: "orange"

    signal clicked()

    Text {
        id: m_text
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: parent.height / 3
        visible: text !== ""
        color: parent.enabled ? isHover ? "red"
                                        : "green"
                              : "blue"
    }

    Image {
        id: m_image
        anchors.centerIn: parent
        width: m_af_button.width * imageCoef
        height: m_af_button.height * imageCoef
        source: isHover && m_af_button.sourceHover !== "" ? m_af_button.sourceHover : m_af_button.source
        visible: source !== ""
        sourceSize.width: m_af_button.width * imageCoef
        sourceSize.height: m_af_button.height * imageCoef
    }

    MouseArea {
        id: m_mouse_area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: /* emit */ m_af_button.clicked()
        onEntered: if (hoverSensitive) isHover = true
        onExited: if (hoverSensitive) isHover = false
    }
}
