import QtQuick 2.9
import QtQuick.Window 2.2
import QtLocation 5.9
import QtPositioning 5.8

Item {
    visible: true
    width: 640
    height: 480

    Plugin {
      id: plugin
      preferred: ["osm"]
      required: Plugin.MappingFeature | Plugin.GeocodingFeature
    }

    Map {
        id: mapItem
        plugin: plugin
        anchors.fill: parent
    }

    function clearMap() {
        mapItem.clearMapItems();
    }

    function setCenter(lat, lon) {
        mapItem.center = QtPositioning.coordinate(lat, lon);
    }

    function addMarker(lat, lon, height, marker, id) {
        var component = Qt.createComponent("qrc:/qml/sign.qml");
        console.log(component.errorString());
        if (component.status === Component.Ready) {
            var sign = component.createObject(parent);
            sign.coordinate = QtPositioning.coordinate(lat, lon);
            sign.heightObstracle = height;
            sign.marker = marker;
            sign.idObstracle = id;
            mapItem.addMapItem(sign);
        }
    }
}
