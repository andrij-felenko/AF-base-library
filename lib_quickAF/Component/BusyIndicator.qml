import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0
import S1 1.0

// TODO rewrite
Item {
    id: busyIndicator
    y: pleaseWaitText.y - height / 2 + pleaseWaitText.height / 2

    property alias timer: timer
    property var colorList: [ // list colors from logo, for busy indicator
        "#FDA21D",
        "#00A779",
        "#0195C3",
        "#006BB9",
        "#444AA2",
        "#822A8E",
        "#D0312B",
        "#FA6736"
    ]

    Timer {
        id: timer
        property real rotate: 0
        interval: 80
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            rotate -= 1
            if (rotate < 0)
                rotate = colorList.length - 1
        }
        onRotateChanged: canvas.requestPaint()
    }

    StackView {
        id: stack
        anchors.fill: parent

        Item {
            anchors.fill: parent

            Canvas {
                id: canvas
                anchors.fill: parent
                property real maxL: Math.min(height, width) / 2
                property real pointInRad: (2 * Math.PI) / colorList.length

                onPaint: {
                    var ctx = canvas.getContext('2d')
                    ctx.reset()

                    for (var it = 0; it < colorList.length; it++){
                        var coef = 1 - (it + timer.rotate) % repeater.count * 0.02
                        var fromCentr = 0.6
                        var cosX = Math.cos(pointInRad * it)
                        var sinX = Math.sin(pointInRad * it)
                        var cosX1 = Math.cos(pointInRad * (it + 1))
                        var sinX1 = Math.sin(pointInRad * (it + 1))

                        ctx.beginPath()
                        ctx.fillStyle = Qt.darker(colorList[it], (it + timer.rotate) % repeater.count / 2 + 0.75)
                        ctx.lineWidth = 2
                        ctx.moveTo(maxL * coef * cosX + maxL, maxL * coef * sinX + maxL)
                        ctx.arc(maxL, maxL, maxL * coef, pointInRad * it, pointInRad * (it + 1))
                        ctx.lineTo(maxL * fromCentr  * cosX1 + maxL, maxL * fromCentr * sinX1 + maxL)
                        ctx.arc(maxL, maxL, maxL * fromCentr, pointInRad * (it + 1), pointInRad * it, true)
//                        ctx.lineTo(maxL * fromCentr  * cosX + maxL, maxL * fromCentr * sinX + maxL)
                        ctx.lineTo(maxL * coef * cosX + maxL, maxL * coef * sinX + maxL)
                        ctx.closePath()
                        ctx.fill()
                    }
                }
            }
        }

        BusyIndicator {
            anchors.fill: parent
//            visible: ! DataHandler.containsOS("macos")
            running: true

            contentItem: Item {
                implicitWidth: 32
                implicitHeight: 32

                Item {
                    id: item
                    anchors.fill: parent
                    opacity: busyIndicator.running ? 1 : 0

                    Repeater {
                        id: repeater
                        model: colorList.length

                        Rectangle {
                            property color trueColor: colorList[index]
                            property int colorShift: (index + timer.rotate) % repeater.count

                            x: (item.width / 2) * (1 + Math.sin(index / repeater.count * 2 * Math.PI)) - width / 2
                            y: (item.height / 2) * (1 + Math.cos(index /  repeater.count * 2 * Math.PI)) - height / 2
                            implicitWidth: item.height * Math.PI / repeater.count / 1.7
                            implicitHeight: item.height * Math.PI / repeater.count / 1.7
                            radius: width / 1.7
                            color: Qt.darker(trueColor, colorShift / 2 + 0.75)
                        }
                    }
                }
            }
        }
    }
}


