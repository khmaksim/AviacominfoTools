#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QVector>

struct ObstraclePoint {
        QString id;
        float lat;
        float lon;
        QString height;
        bool marker;
        bool type;
};

class MapView : public QQuickView
{
        Q_OBJECT

    public:
        explicit MapView();
        ~MapView() {}

        void setRadius(QVariant);
        void clearMap();
        void setCenter(QVariant);
        void addObstracle(ObstraclePoint);

    signals:
        void checked(bool, QString);
};

#endif // MAPVIEW_H
