#include "databaseupdate.h"
#include "databaseaccess.h"

DatabaseUpdate::DatabaseUpdate(QObject *parent) : QObject(parent)
{

}

void DatabaseUpdate::process()
{
    DatabaseAccess::getInstance()->update(airfield, obstracles);
    emit finished();
}

void DatabaseUpdate::setData(QMap<QString, QString> airfield, QVector<QVector<QString>> obstracles)
{
    this->airfield = airfield;
    this->obstracles = obstracles;
}

void DatabaseUpdate::stop()
{
    return;
}
