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
        void checkUpdates();

    private:
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QVector<QVector<QString>> obstracles;
        QMap<QString, QString> airfields;
        QVector<QString> href;
        QStringList hrefPages;
        bool lastPage;

        void getAirfields();
        void parser(QByteArray&, HtmlParser::TypeData);
        void getHtmlPage(QUrl);

    private slots:
        void replyFinished(QNetworkReply*);
        void updateProgress(qint64,qint64);
        void getObstracles();
        void storeAirfields(QMap<QString, QString>, QVector<QString>);
        void storeObstracles(QVector<QVector<QString>>);
        void storeHrefPages(QStringList);
        void updateDatabase();

    signals:
        void updated();
};

#endif // OBSTRACLESHANDLER_H
