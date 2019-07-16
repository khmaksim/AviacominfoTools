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

        void setTag(const QString&, const QVariantList&);
        QVector<QString> getTags();
        QVector<QString> getTypeObstracle();
        QVector<QVariantList> getAirfields();
        QVector<QVariantList> getObstracles(uint);
        bool createTag(const QString&);

    public slots:
        void update(QMap<QString, QString>, QVector<QVector<QString>>);

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);
//        static DatabaseAccess *databaseAccess;

        QSqlDatabase db;

    signals:
        void updated();
};

#endif // DATABASEACCESS_H
