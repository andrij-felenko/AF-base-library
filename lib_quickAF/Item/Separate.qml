import QtQuick 2.0
import qscolors 1.0

// TODO rewrite
Item {
    height: 1
    property alias lineHeight: m_rect.height

    Rectangle {
        id: m_rect
        width: parent.width
        y: (parent.height - height) / 2
        color: S1Colors.separate
        height: 1
    }
}
