#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QVector>

struct ObstraclePoint {
    double b;       // geodetic latitude
    double l;       // geodetic longitude
    int height;
};

class MapView : public QQuickView
{
        Q_OBJECT

    public:
        explicit MapView(QVariant);
        ~MapView() {}

        void setData(QVector<ObstraclePoint>);

    private:
        QVector<ObstraclePoint> obstracles;
};

#endif // MAPVIEW_H
