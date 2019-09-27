#include "databaseupdate.h"
#include "databaseaccess.h"

DatabaseUpdate::DatabaseUpdate(QObject *parent) : QObject(parent)
{

}

void DatabaseUpdate::process()
{
    DatabaseAccess::getInstance()->update(icaoCodeAirfield, nameAirfield, obstracles);
    emit finished();
}

void DatabaseUpdate::setData(const QString &icaoCodeAirdield, const QString &nameAirdield, QVector<QVector<QString>> &obstracles)
{
    this->icaoCodeAirfield = icaoCodeAirdield;
    this->nameAirfield = nameAirdield;
    this->obstracles = obstracles;
}

void DatabaseUpdate::stop()
{
    return;
}
