#ifndef OBSTRACLESGRAPHICSVIEW_H
#define OBSTRACLESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QVector>

struct ObstraclesPoint {
    float x;
    float y;
};

class ObstraclesGraphicsView : public QGraphicsView
{
        Q_OBJECT

    public:
        explicit ObstraclesGraphicsView(QWidget *parent = 0);
        ~ObstraclesGraphicsView() {}

        void setData(QVector<QVector<QString>>);

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        QGraphicsScene *scene;
        QTimer *timer;
        QVector<ObstraclesPoint> obstracles;

        float convertCoordInDec(QString);

    private slots:
        void setObstracles();
};

#endif // OBSTRACLESGRAPHICSVIEW_H
