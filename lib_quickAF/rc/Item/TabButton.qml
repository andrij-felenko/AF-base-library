import QtQuick 2.4
import QtQuick.Controls 2.2
import qscolors 1.0

// TODO rewrite
TabButton {
    id: control
    property bool isCheckedOpacity: true
    clip: true
    height: parent.height
    implicitHeight: height

    contentItem: Text {
        text: control.text
        font.pixelSize: parent.height * 0.4
        color: control.checked ? S1Colors.textEnabled : S1Colors.textDisabled
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    function repaintCanvas(){
        canvas.requestPaint()
    }

    background: Item {
        Canvas {
            id: canvas
            anchors.fill: parent
            antialiasing: true

            onPaint: {
                var ctx = canvas.getContext('2d')
                var radius = control.height / 4
                var cWidth = Math.ceil(control.width)
                var cHeight = control.height

                ctx.reset()
                ctx.fillStyle   = control.checked ? S1Colors.checkedTarget : S1Colors.uncheckedTarget
                ctx.strokeStyle = control.checked ? S1Colors.checkedTarget : "transparent"
                ctx.lineWidth = 1

                ctx.beginPath()
                ctx.moveTo(0, cHeight)
                ctx.lineTo(0, radius)
                ctx.bezierCurveTo(0, radius, 0, 0, radius, 0);
                ctx.lineTo(cWidth - radius, 0)
                ctx.bezierCurveTo(cWidth - radius, 0, cWidth, 0, cWidth, radius);
                ctx.lineTo(cWidth, cHeight)


                // draw rectangle
                ctx.fill()
                if (control.checked)
                    ctx.stroke();
                ctx.restore();
            }
        }
    }
}
