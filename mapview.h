#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QVector>

struct ObstraclePoint {
    double lat;
    double lon;
    int height;
};

class MapView : public QQuickView
{
        Q_OBJECT

    public:
        explicit MapView(QVariant);
        ~MapView() {}

        void addObstracle(ObstraclePoint);
};

#endif // MAPVIEW_H
