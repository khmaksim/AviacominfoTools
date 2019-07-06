#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include "obstracleshandler.h"

class DatabaseAccess : public QObject
{
        Q_OBJECT
    public:
        static DatabaseAccess* getInstance();

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);

        QSqlDatabase db;

    public:
        QVector<Airfield> getAirfields();
        QVector<QVariantList> getObstracles(uint);

    public slots:
        void update(Airfield, QVector<QVector<QString> >&);
};

#endif // DATABASEACCESS_H
