import QtQuick 2.9
import QtQuick.Window 2.2
import QtLocation 5.9
import QtPositioning 5.8

Item {
    id: root
    visible: true
    width: 640
    height: 480
    signal selected(string id);

    Plugin {
      id: plugin
      preferred: ["osm"]
      required: Plugin.AnyMappingFeatures | Plugin.AnyGeocodingFeatures
    }

    Map {
        id: mapView
        plugin: plugin
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent

            onDoubleClicked: {
                var coordinate = mapView.toCoordinate(Qt.point(mouse.x,mouse.y))
                var numItems = mapView.mapItems.length;

                for (var i = 0; i < numItems; i++) {
                    var coordinateObstracle = mapView.mapItems[i].coordinate;
                    var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
                                                           Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
                                                           Math.cos(degreesToRadians(coordinate.latitude)) *
                                                           Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
                                                                                                       coordinateObstracle.longitude) / 2)), 2)));
                    if (d <= 0.05) {
                        mapView.mapItems[i].selected = !mapView.mapItems[i].selected;
                        root.selected(mapView.mapItems[i].idObstracle);
                        break;
                    }
                }
            }
        }
    }

    function degreesToRadians(degrees) {
        return (degrees * Math.PI)/180;
    }

    function clearMap() {
        mapView.clearMapItems();
    }

    function setCenter(lat, lon) {
        mapView.center = QtPositioning.coordinate(lat, lon);
    }

    function addMarker(lat, lon, height, marker, id) {
        var component = Qt.createComponent("qrc:/qml/sign.qml");
        if (component.status === Component.Ready) {
            var sign = component.createObject(parent);
            sign.coordinate = QtPositioning.coordinate(lat, lon);
            sign.heightObstracle = height;
            sign.marker = marker;
            sign.idObstracle = id;
            mapView.addMapItem(sign);
        }
    }
}
