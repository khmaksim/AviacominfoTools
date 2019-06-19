#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "obstracleshandler.h"

class Database : public QObject
{
        Q_OBJECT
    public:
        explicit Database(QObject *parent = nullptr);

    public slots:
        void update(Airfield, QVector<QVector<QString> >&);
};

#endif // DATABASE_H
