#include "obstracleshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QCoreApplication>
#include "databaseaccess.h"
#include "databaseupdate.h"

ObstraclesHandler::ObstraclesHandler(QObject *parent) : QObject(parent)
{
    lastPage = false;
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
        parser(data, HtmlParser::Airfields);
    }
    else {
        QByteArray data = reply->readAll();
        parser(data, HtmlParser::Obstracles);
    }
    reply->deleteLater();
}

void ObstraclesHandler::checkUpdates()
{
    getAirfields();
}

void ObstraclesHandler::getHtmlPage(QUrl url)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/html")));
    request.setUrl(url);
    reply = manager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(updateProgress(qint64,qint64)));
}

void ObstraclesHandler::getAirfields()
{
    getHtmlPage(QUrl("http://www.caica.ru/ObstacleList/rus/"));
}

void ObstraclesHandler::getObstracles()
{
    if (!hrefPages.isEmpty()) {
        getHtmlPage(QUrl("http://www.caica.ru/ObstacleList/rus/").resolved(QUrl(hrefPages.takeFirst())));
        if (hrefPages.isEmpty())
            lastPage = true;
        return;
    }
    lastPage = false;

    if (obstracles.size() > 0) {
        updateDatabase();
        obstracles.clear();
        return;
    }

    if (airfields.isEmpty()) {
        emit updated();
        return;
    }

    // get first html page from list href
    getHtmlPage(QUrl("http://www.caica.ru/ObstacleList/rus/").resolved(QUrl(href.first())));
}

void ObstraclesHandler::updateProgress(qint64 r, qint64 t)
{
    qDebug() << r << " " << t;
}

void ObstraclesHandler::parser(QByteArray &data, HtmlParser::TypeData type)
{
    HtmlParser *parser = new HtmlParser(type);
    QThread *thread = new QThread;
    parser->setData(data);
    parser->moveToThread(thread);

    qRegisterMetaType<QVector<QMap<QString,QString>>>();
    qRegisterMetaType<QVector<QVector<QString>>>();
    connect(thread, SIGNAL(started()), parser, SLOT(process()));
    connect(parser, SIGNAL(finished()), thread, SLOT(quit()));
    connect(parser, SIGNAL(parsedAirfields(QMap<QString, QString>,QVector<QString>)), this, SLOT(storeAirfields(QMap<QString, QString>, QVector<QString>)));
    connect(parser, SIGNAL(parsedAirfields(QMap<QString, QString>,QVector<QString>)), this, SLOT(getObstracles()));
    connect(parser, SIGNAL(parsedPagins(QStringList)), this, SLOT(storeHrefPages(QStringList)));
    connect(parser, SIGNAL(parsedObstracles(QVector<QVector<QString>>)), this, SLOT(storeObstracles(QVector<QVector<QString>>)));
    connect(parser, SIGNAL(parsedObstracles(QVector<QVector<QString>>)), this, SLOT(getObstracles()));
    connect(parser, SIGNAL(finished()), parser, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void ObstraclesHandler::storeHrefPages(QStringList hrefPages)
{
    if (!this->hrefPages.isEmpty() || lastPage)
        return;
    this->hrefPages = hrefPages;
}


void ObstraclesHandler::storeAirfields(QMap<QString, QString> airfields, QVector<QString> href)
{
    this->airfields = airfields;

    href.append("enrt.html");
    this->href = href;
}

void ObstraclesHandler::storeObstracles(QVector<QVector<QString>> obstracles)
{
    this->obstracles += obstracles;
}

void ObstraclesHandler::updateDatabase()
{
    DatabaseUpdate *update = new DatabaseUpdate;
    QThread *thread = new QThread;

    QMap<QString, QString>::iterator it = airfields.erase(airfields.begin());
    update->setData(it.key(), it.value(), obstracles);
    update->moveToThread(thread);

    connect(thread, SIGNAL(started()), update, SLOT(process()));
    connect(update, SIGNAL(finished()), thread, SLOT(quit()));
    connect(update, SIGNAL(finished()), this, SLOT(getObstracles()));
    connect(update, SIGNAL(finished()), update, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
