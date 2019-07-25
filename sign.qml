import QtQuick 2.0
import QtLocation 5.9

MapQuickItem {
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height
    property alias heightObstracle: labelHeight.text
    property bool marker: false
    property alias idObstracle: labelId.text

    sourceItem: Column {
        Row {
            Image {
                id: image
                source: marker ? "qrc:/images/res/img/W.png" : "qrc:/images/res/img/A.png"
                width: 20
                height: 30
            }
            Text {
                id: labelHeight
            }
        }
        Text {
            id: labelId
        }
    }
}
