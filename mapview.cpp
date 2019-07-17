#include "mapview.h"
#include <QQuickItem>
#include <QTimer>
#include <QDebug>

MapView::MapView(QVariant coordinate) : QQuickView()
{
    setResizeMode(QQuickView::SizeRootObjectToView);
    setSource(QUrl::fromLocalFile(":/qml/mapview.qml"));

    double lat = coordinate.toPointF().x();
    double lon = coordinate.toPointF().y();

    QMetaObject::invokeMethod(rootObject(), "setCenter", Qt::DirectConnection,
                              Q_ARG(QVariant, QVariant(lat)),
                              Q_ARG(QVariant, QVariant(lon)));
}

void MapView::addObstracle(ObstraclePoint obstracle)
{
    QMetaObject::invokeMethod(rootObject(), "addMarker", Qt::DirectConnection,
                              Q_ARG(QVariant, QVariant(obstracle.lat)),
                              Q_ARG(QVariant, QVariant(obstracle.lon)),
                              Q_ARG(QVariant, QVariant(QString::number(obstracle.height))));
}
