#include "obstracleshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

ObstraclesHandler::ObstraclesHandler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/xml")));
    request.setUrl(QUrl("http://www.caica.ru/ObstacleList/rus/"));
    reply = manager->get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(updateProgress(qint64,qint64)));
}

ObstraclesHandler::~ObstraclesHandler()
{
    delete manager;
}

void ObstraclesHandler::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
        qDebug() << "Error: " << reply->error() << reply->errorString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    else {
        QByteArray data = reply->readAll();
        int pos = 0;
//        int start = -1;
//        int end = -1;
        QString startMenu = "<div class=\"menu\">";
//        QString startItemMenu = "<div class=\"menu_item\">";
//        QString endItemMenu = "</div>";
        QRegExp regExp("<a target=\"view_frame\" href=\"([^\"]+)\">([^<]+)<br /><b>([^<]+)</b></a>");

        if ((pos = data.indexOf(startMenu, pos)) > 0) {
//            pos = data.indexOf(startItemMenu, pos);
//            if (pos < 0)
//                break;
//            start = pos + startItemMenu.length();
//            pos = data.indexOf(endItemMenu, pos);
//            if (pos < 0)
//                break;
//            end = pos;

//            int p = 0;
//            while ((p = regExp.indexIn(data.mid(start, end), p)) != -1) {
            while ((pos = regExp.indexIn(data, pos)) != -1) {
                pos += regExp.matchedLength();
                Airfield airfield;
                airfield.href = regExp.cap(1);
                airfield.name = regExp.cap(2);
                airfield.icao = regExp.cap(3);
//                qDebug() << airfield.name;
                airfields << airfield;
            }
        }
        emit finished(airfields);
    }
    reply->deleteLater();
}

void ObstraclesHandler::updateProgress(qint64 r, qint64 t)
{
    qDebug() << r << " " << t;
}
