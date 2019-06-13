#include "obstracleshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

ObstraclesHandler::ObstraclesHandler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    QNetworkRequest request;
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/html")));
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
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error: " << reply->error() << reply->errorString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        manager->get(reply->request());
    }
    else if (reply->url() == QUrl("http://www.caica.ru/ObstacleList/rus/")) {
        QByteArray data = reply->readAll();
        int pos = 0;
        QVector<Airfield> airfields;
//        int start = -1;
//        int end = -1;
        QString startMenu = "<div class=\"menu\">";
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
                airfields << airfield;
            }
        }
        emit finished(airfields);
    }
    else {
        QByteArray data = reply->readAll();
        int pos = 0;
        QRegExp regExp("<td>([^<]*)</td>");
        QVector<QVector<QString>> table;
        QVector<QString> row;

        int col = 0;
        while ((pos = regExp.indexIn(data, pos)) != -1) {
            pos += regExp.matchedLength();
            row.append(regExp.cap(1));
            col++;

            if (col == 26) {
                col = 0;
                table << row;
                row.clear();
            }
        }
        emit finished(table);
    }
    reply->deleteLater();
}

void ObstraclesHandler::getListObstracles(const QString &file)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/html")));
    request.setUrl(QUrl("http://www.caica.ru/ObstacleList/rus/").resolved(QUrl(file)));

    reply = manager->get(request);
}

void ObstraclesHandler::updateProgress(qint64 r, qint64 t)
{
    qDebug() << r << " " << t;
}
