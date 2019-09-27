import QtQuick 2.0
import QtLocation 5.9

MapQuickItem {
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height
    property alias heightObstracle: labelHeight.text
    property bool marker: false
    property bool type: false
    property alias idObstracle: labelId.text
    property bool selected: false

    sourceItem: Column {
        Row {
            Image {
                id: image
                source: type ? "qrc:/images/res/img/P.png" : (marker ? "qrc:/images/res/img/W.png" : "qrc:/images/res/img/A.png")
                width: type ? 5 : 20
                height: type ? 5 : 30
            }
            Text {
                id: labelHeight
            }
        }
        Text {
            id: labelId
            color: selected ? "red" : "black"
        }
    }
}
