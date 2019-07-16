import QtQuick 2.9
import QtQuick.Window 2.2
import QtLocation 5.9
import QtPositioning 5.8

Item {
    visible: true
    width: 640
    height: 480
//    title: qsTr("Hello World")

    Plugin {
      id: plugin
      preferred: ["osm"]
      required: Plugin.MappingFeature | Plugin.GeocodingFeature
    }

    Map {
        id: mapItem
        plugin: plugin
        anchors.fill: parent

//        MapQuickItem {
//            id: marker
//            anchorPoint.x: image.width/4
//            anchorPoint.y: image.height

//            sourceItem: Image {
//                id: image
//            }
//        }
    }

    function setCenter(lat, lon) {
        mapItem.center = QtPositioning.coordinate(lat, lon);
    }
}
