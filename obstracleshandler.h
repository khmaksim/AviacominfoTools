#ifndef OBSTRACLESHANDLER_H
#define OBSTRACLESHANDLER_H

#include <QWidget>

struct Airfield {
    QString name;
    QString icao;
    QString href;
};

class QNetworkReply;
class QNetworkAccessManager;
class ObstraclesHandler : public QObject
{
        Q_OBJECT
    public:
        explicit ObstraclesHandler(QObject *parent = nullptr);
        ~ObstraclesHandler();

    public slots:
        void replyFinished(QNetworkReply*);
        void updateProgress(qint64,qint64);

    private:
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QVector<Airfield> airfields;

    signals:
        void finished(QVector<Airfield>&);
};

#endif // OBSTRACLESHANDLER_H
