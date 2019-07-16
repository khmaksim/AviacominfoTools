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

void MapView::setData(QVector<ObstraclePoint> obstracles)
{
    this->obstracles = obstracles;
}
