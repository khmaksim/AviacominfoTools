#ifndef OBSTRACLESHANDLER_H
#define OBSTRACLESHANDLER_H

#include <QWidget>

struct Airfield {
    QString name;
    QString icao;
    QString href;
    uint id;
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
        void checkUpdates();

    private:
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QVector<Airfield> airfields;

        void getListObstracles(const QString&);
        void getListAirfields();

    private slots:
        void replyFinished(QNetworkReply*);
        void updateProgress(qint64,qint64);

    signals:
        void finished(Airfield, QVector<QVector<QString>>&);
};

#endif // OBSTRACLESHANDLER_H
