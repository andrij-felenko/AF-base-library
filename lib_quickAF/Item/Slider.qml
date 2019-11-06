import QtQuick 2.9
import QtQuick.Controls 2.4
import S1 1.0

// TODO rewrite
Item {
    anchors.fill: parent

    property alias sliderItem: slider
    property ModelSlider modelSlider: null
    signal shifted(real newValue)
    
    TrayText {
        id: sliderValueText
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        width: 20
        textItem.verticalAlignment: Text.AlignVCenter
        
        text: Math.round(slider.value * 10) / 10
        onTextChanged: /* emit */ shifted(Number(text))
    }
    
    Slider {
        id: slider
        anchors.left: sliderValueText.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        
        handle.height: parent.height * 0.7
        handle.width: handle.height
        snapMode: Slider.SnapAlways
        stepSize: to - from > 150 ? 10 : to - from > 75 ? 5 : to - from > 10 ? 1 : 0.1
        
        from: 0
        to: 1
        value: 0.5
    }
}
