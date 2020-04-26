import QtQuick 2.9
import QtQuick.Controls 2.2
import qscolors 1.0

// TODO rewrite
TextInputRectangle {
    property alias text_input: m_text_input

    input: TextInput {
        id: m_text_input
        anchors.fill: parent
        selectByMouse: true
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        font.letterSpacing: 2
        font.pixelSize: height * 0.6
        maximumLength: 7
        onTextEdited: {
            var validText = DataHandler.convertToDigit(text)
            if (validText === text){
                /* emit */ currentChanged(String(Number(text)))
                /* emit */ currentTextChanged(text)
            }
            else
                text = validText
        }
    }
}
