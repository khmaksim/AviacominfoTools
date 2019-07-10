#include "obstraclesgraphicsview.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

ObstraclesGraphicsView::ObstraclesGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setMinimumHeight(400);
    this->setMinimumWidth(400);

    scene = new QGraphicsScene();
    this->setScene(scene);

    timer = new QTimer();
    timer->setSingleShot(true);

    connect(timer, SIGNAL(timeout()), this, SLOT(setObstracles()));

    timer->start(50);
}

void ObstraclesGraphicsView::resizeEvent(QResizeEvent *event)
{
    timer->start(50);
    QGraphicsView::resizeEvent(event);
}

void ObstraclesGraphicsView::setObstracles()
{
    scene->clear();

    int width = this->width();
    int height = this->height();

    scene->setSceneRect(0, 0, width, height);

    QPen penBlack(Qt::black);
    QPen penRed(Qt::red);

    for (int i = 0; i < obstracles.size(); i++) {
        scene->addEllipse(obstracles.at(i).x, obstracles.at(i).y, 5, 5, penBlack, QBrush(Qt::red));
        qDebug() << obstracles.at(i).x << obstracles.at(i).y;
    }
}

void ObstraclesGraphicsView::setData(QVector<QVector<QString>> coordObstracles)
{
    for (int i = 0; i < coordObstracles.size(); i++) {
        ObstraclesPoint obstracle;
        obstracle.x = convertCoordInDec(coordObstracles.at(i).first());
        obstracle.y = convertCoordInDec(coordObstracles.at(i).last());
        obstracles.append(obstracle);
    }
}

float ObstraclesGraphicsView::convertCoordInDec(QString coordStr)
{
    QRegExp regExp("\\S(\\d{1,3})\\s(\\d{1,2})\\s([\\d\\.]+)");
    float coordDec = 0;

    if (regExp.indexIn(coordStr) != -1) {
        coordDec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).toFloat() / 3600;
    }

    return coordDec;
}
