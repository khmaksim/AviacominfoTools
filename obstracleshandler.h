#ifndef OBSTRACLESHANDLER_H
#define OBSTRACLESHANDLER_H

#include <QWidget>
#include <QMap>
#include "htmlparser.h"

class QNetworkReply;
class QNetworkAccessManager;
class ObstraclesHandler : public QObject
{
        Q_OBJECT
    public:
        explicit ObstraclesHandler(QObject *parent = nullptr);
        ~ObstraclesHandler();

    public slots:
        void update();

    private:
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QVector<QVector<QString>> obstracles;
        QVector<Airfield> airfields;
        Airfield airfield;
        QStringList hrefPages;
        bool lastPage;
        bool isProcessUpdate;

        void getAirfields();
        void parser(QByteArray&, HtmlParser::TypeData);
        void getHtmlPage(QUrl);

    private slots:
        void replyFinished(QNetworkReply*);
        void updateProgress(qint64,qint64);
        void getObstracles();
        void storeAirfields();
        void storeObstracles(QVector<QVector<QString>>);
        void storeHrefPages(QStringList);
        void updateDatabase();

    signals:
        void updated();
};

#endif // OBSTRACLESHANDLER_H
