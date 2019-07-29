#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QVector>

struct ObstraclePoint {
        QString id;
        double lat;
        double lon;
        QString height;
        bool marker;
};

class MapView : public QQuickView
{
        Q_OBJECT

    public:
        explicit MapView();
        ~MapView() {}

        void clearMap();
        void setCenter(QVariant);
        void addObstracle(ObstraclePoint);

//    public slots:
//        void
        signals:
        void selected(QString);
};

#endif // MAPVIEW_H
