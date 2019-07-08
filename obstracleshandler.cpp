#include "obstracleshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include "databaseaccess.h"

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
        createParser(data, HtmlParser::Airfields);
    }
    else {
        QByteArray data = reply->readAll();
        createParser(data, HtmlParser::Obstracles);
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

void ObstraclesHandler::createParser(QByteArray &data, HtmlParser::TypeData type)
{
    HtmlParser *parser = new HtmlParser(type);
    QThread *thread = new QThread;
    parser->setData(data);
    parser->moveToThread(thread);

    qRegisterMetaType<QVector<QVector<QString>>>("QVector<QVector<QString>>");
    connect(thread, SIGNAL(started()), parser, SLOT(process()));
    connect(parser, SIGNAL(finished()), thread, SLOT(quit()));
    connect(parser, SIGNAL(parseAirfieldCompleted(QString)), this, SLOT(getListObstracles(QString)));
    connect(parser, SIGNAL(parseObstraclesCompleted(QVector<QVector<QString>>)), DatabaseAccess::getInstance(), SLOT(update(QVector<QVector<QString>>)));
    connect(parser, SIGNAL(finished()), parser, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
