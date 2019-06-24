#include "obstracleshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

ObstraclesHandler::ObstraclesHandler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

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
        QString startMenu = "<div class=\"menu\">";
        QRegExp regExp("<a target=\"view_frame\" href=\"([^\"]+)\">([^<]+)<br /><b>([^<]+)</b></a>");

        if ((pos = data.indexOf(startMenu, pos)) > 0) {
            while ((pos = regExp.indexIn(data, pos)) != -1) {
                pos += regExp.matchedLength();
                QString href = regExp.cap(1);
                Airfield airfield;
                airfield.name = regExp.cap(2);
                airfield.icao = regExp.cap(3);
                airfields << airfield;

                getListObstracles(href);
            }
        }
    }
    else {
        QByteArray data = reply->readAll();
        int pos = 0;
        QRegExp tableBeginRegExp("<tbody");
        QRegExp rowBeginRegExp("<tr[^>]*>");
        QRegExp rowEndRegExp("</tr>");
        QRegExp colRegExp("<td>([^<]*)</td>");
        QVector<QVector<QString>> table;
        QVector<QString> row;
        int posEndRow = 0;

        data.replace("<td />", "<td></td>");        // fixed syntax error
        pos = tableBeginRegExp.indexIn(data, pos);      // position begin table body

        if (pos != -1) {
            while ((pos = rowBeginRegExp.indexIn(data, pos)) != -1) {
                posEndRow = rowEndRegExp.indexIn(data, pos);

                if (posEndRow == -1)
                    continue;

                while ((pos = colRegExp.indexIn(data, pos)) != -1) {
                    pos += colRegExp.matchedLength();

                    if (pos > posEndRow) {
                        pos = posEndRow;
                        table << row;
                        row.clear();
                        break;
                    }
                    row.append(colRegExp.cap(1));
                }
                //  add table last row
                if (!row.isEmpty())
                    table << row;
            }
            emit finished(airfields.takeFirst(), table);
        }
    }
    reply->deleteLater();
}

void ObstraclesHandler::checkUpdates()
{
    getListAirfields();
}

void ObstraclesHandler::getListAirfields()
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/html")));
    request.setUrl(QUrl("http://www.caica.ru/ObstacleList/rus/"));
    reply = manager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(updateProgress(qint64,qint64)));
}

void ObstraclesHandler::getListObstracles(const QString &file)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/html")));
    request.setUrl(QUrl("http://www.caica.ru/ObstacleList/rus/").resolved(QUrl(file)));
    reply = manager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(updateProgress(qint64,qint64)));
}

void ObstraclesHandler::updateProgress(qint64 r, qint64 t)
{
    qDebug() << r << " " << t;
}
