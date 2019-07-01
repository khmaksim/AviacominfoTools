#include "sortsearchfiltertablemodel.h"
#include "listitemdelegate.h"
#include <QDebug>
#include <QtMath>

SortSearchFilterTableModel::SortSearchFilterTableModel(QObject *parent)
{
    Q_UNUSED(parent)
    markingDay = false;
    nightMarking = false;
    lat = 0;
    lon = 0;
    radius = 0;
}

SortSearchFilterTableModel::~SortSearchFilterTableModel()
{
}

bool SortSearchFilterTableModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    bool result = false;

    for (int col = 1; col < sourceModel()->columnCount(); col++) {
        QModelIndex index = sourceModel()->index(sourceRow, col, sourceParent);
        result |= sourceModel()->data(index).toString().contains(filterRegExp());
    }

    if (markingDay)
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 17, sourceParent))
                  .toString().contains(QRegExp("да|есть"));

    if (nightMarking)
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 20, sourceParent))
                  .toString().contains(QRegExp("да|есть"));

    if (lat > 0 && lon > 0 && radius > 0) {
        float latObstracle = convertCoordInDec(sourceModel()->data(sourceModel()->index(sourceRow, 6, sourceParent)).toString());
        float lonObstracle = convertCoordInDec(sourceModel()->data(sourceModel()->index(sourceRow, 7, sourceParent)).toString());

        if (latObstracle > 0 && lonObstracle > 0) {
//            double d = 2 * qAcos(qSin(lat) * qSin(latObstracle) + qCos(lat) * qCos(latObstracle) * qCos(lon - lonObstracle));
            double d = 2 * qAsin(qSqrt(qPow(qSin((latObstracle - lat) / 2), 2) + qCos(lat) * qCos(latObstracle) * qPow(qSin((lon - lonObstracle) / 2), 2)));
            if ((d * 6372795) <= (radius * 1000))
                result &= true;
            else
                result &= false;
        }
    }

    return result;
}

bool SortSearchFilterTableModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

//    if (leftData.type() == QVariant::Int) {
    if (leftData.toString().contains(QRegExp("^\\d+\\.?\\d*$")) &&
            rightData.toString().contains(QRegExp("^\\d+\\.?\\d*$"))) {
        return leftData.toInt() < rightData.toInt();
    } /*else {
        static const QRegularExpression emailPattern("[\\w\\.]*@[\\w\\.]*");

        QString leftString = leftData.toString();
        if (left.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(leftString);
            if (match.hasMatch())
                leftString = match.captured(0);
        }
        QString rightString = rightData.toString();
        if (right.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(rightString);
            if (match.hasMatch())
                rightString = match.captured(0);
        }

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }*/
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void SortSearchFilterTableModel::setFilterProperty(QString objectName, bool flag)
{
    if (objectName.contains("day", Qt::CaseInsensitive))
        markingDay = flag;
    else if (objectName.contains("night", Qt::CaseInsensitive))
        nightMarking = flag;

    invalidateFilter();
}

void SortSearchFilterTableModel::setFilterRadius(float lat, float lon, int radius)
{
    this->lat = lat;
    this->lon = lon;
    this->radius = radius;

    invalidateFilter();
}

float SortSearchFilterTableModel::convertCoordInDec(const QString coordStr) const
{
    QRegExp regExp("\\S(\\d{1,3})\\s(\\d{1,2})\\s([\\d\\.]+)");
    float coordDec = 0;

    if (regExp.indexIn(coordStr) != -1)
        coordDec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).toFloat() / 3600;

    return coordDec;
}
