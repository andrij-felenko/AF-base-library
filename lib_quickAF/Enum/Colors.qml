pragma Singleton
import QtQuick 2.11

// TODO rewrite
QtObject {
    readonly property color base: "#ffffff"
    readonly property color separate: "#595959"

    readonly property color textEnabled: "#000000"
    readonly property color textDisabled: "#ffffff"
    readonly property color textAlternate: "#4d4d4d"

    readonly property color buttonDisasbled: "#f0f0f0"
    readonly property color buttonHover: "#c0c0c0"
    readonly property color button: "#ffffff"

    readonly property color buttonTextDisabled: "#808080"
    readonly property color buttonTextHover: "#ffffff"
    readonly property color buttonText: "#000000"

    readonly property color   checkedTarget: "#efefef"
    readonly property color uncheckedTarget: "#dadada"

    readonly property color panel: "#c5c5c5"
    readonly property color panelText: "#bababa"

    readonly property color alternatePanel: "#939393"
    readonly property color alternatePanelText: "#ffffff"

    readonly property color fieldBackground: "#ffffff"
    readonly property color fieldBorder: "#c0c0c0"

    readonly property color modalWindowBackground: "#000000"
    readonly property color modalWindowText: "#ffffff"
}
