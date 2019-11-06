import QtQuick 2.9
import QtQuick.Controls 2.2
import S1 1.0
import qscolors 1.0

// TODO rewrite
ComboBox {
    id: m_settings_combobox
    anchors.fill: parent
    font.pixelSize: height *0.45
    implicitHeight: parent.height

    delegate: ItemDelegate {
        id: delegate
        height: m_settings_combobox.height * (DataHandler.containsOS("macos") ? 1.2 : 1.4)
        width: m_settings_combobox.width
        highlighted: m_settings_combobox.highlightedIndex === index

        contentItem: Text {
            anchors.fill: parent
            text: modelData
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            enabled: m_settings_combobox.enabled
            font.pixelSize: delegate.height * (DataHandler.containsOS("macos") ? 0.4 : 0.35)
        }
    }

    property ModelStringList stringModel: null
    property string title: stringModel === null ? "" : stringModel.title
    model: stringModel === null ? undefined : stringModel.list
    displayText: stringModel === null ? "" : stringModel.cutFlags(stringModel.current)

    popup.onOpened: m_indicator.isDropDownOpen = true
    popup.onClosed: m_indicator.isDropDownOpen = false
    onActivated: {
        var it = currentIndex
        stringModel.current = stringModel.cutFlags(currentText)
        currentIndex = it
    }

    background: Rectangle {
        color: S1Colors.fieldBackground
        border.width: 1
        border.color: S1Colors.fieldBorder
        radius: 3
    }

    indicator: Item {
        id: m_indicator
        property bool isDropDownOpen: false
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: height
        enabled: m_settings_combobox.enabled
        visible: enabled

        Image {
            anchors.fill: parent
            anchors.margins: parent.height / 3
            sourceSize.width: width
            sourceSize.height: height
            enabled: m_settings_combobox.enabled

            rotation: m_indicator.isDropDownOpen ? 180 : 0
            source: Qt.resolvedUrl("qrc:/view/images/drop down arrow.svg")
        }
    }

    contentItem: Text {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
        enabled: m_settings_combobox.enabled
        color: enabled ? S1Colors.textEnabled : S1Colors.textAlternate
        text: displayText
        font.pixelSize: m_settings_combobox.height * 0.5
    }
}
