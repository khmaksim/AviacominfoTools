#ifndef OBSTRACLESHANDLER_H
#define OBSTRACLESHANDLER_H

#include <QWidget>
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

        void getListAirfields();
        void createParser(QByteArray&, HtmlParser::TypeData);

    private slots:
        void replyFinished(QNetworkReply*);
        void updateProgress(qint64,qint64);
        void getListObstracles(const QString&);
};

#endif // OBSTRACLESHANDLER_H
