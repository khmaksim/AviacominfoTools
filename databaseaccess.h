#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include "obstracleshandler.h"

struct Airfield {
    QString name;
    QString icao;
    QString href;
    uint id;
};

class DatabaseAccess : public QObject
{
        Q_OBJECT
    public:
        static DatabaseAccess* getInstance();

        void setTag(const QString&, const QVariantList&);
        QVector<QString> getTags();
        QVector<Airfield> getAirfields();
        QVector<QVariantList> getObstracles(uint);
        bool createTag(const QString&);
        void addAirfield(Airfield);

    public slots:
        void update(QVector<QVector<QString>>);

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);

        QSqlDatabase db;
        QVector<Airfield> airfields;

    signals:
        void updated();
};

#endif // DATABASEACCESS_H
