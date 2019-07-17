import QtQuick 2.0
import QtLocation 5.9

MapQuickItem {
    id: marker
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height
    property alias label: labelHeight.text

    sourceItem: Row {
        Image {
            id: image
            source: "qrc:/images/res/img/A.png"
            width: 20
            height: 20
        }
        Text {
            id: labelHeight
        }
    }
}
