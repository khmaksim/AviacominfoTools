#include "sortsearchfilterobstraclemodel.h"
#include "listitemdelegate.h"
#include <QDebug>
#include <QtMath>
#include <QString>

SortSearchFilterObstracleModel::SortSearchFilterObstracleModel(QObject *parent)
{
    Q_UNUSED(parent)
    markingDay = false;
    nightMarking = false;
    lat = 0;
    lon = 0;
    radius = 0;
    fromHeight = 0;
    toHeight = 0;
}

SortSearchFilterObstracleModel::~SortSearchFilterObstracleModel()
{
}

bool SortSearchFilterObstracleModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    bool result = false;

    for (int col = 1; col < sourceModel()->columnCount(); col++) {
        QModelIndex index = sourceModel()->index(sourceRow, col, sourceParent);
        result |= sourceModel()->data(index).toString().contains(filterRegExp());
    }

    if (fromHeight > 0) {
        result &= (fromHeight <= sourceModel()->data(sourceModel()->index(sourceRow, 12, sourceParent)).toInt());
    }

    if (toHeight > 0) {
        result &= (toHeight >= sourceModel()->data(sourceModel()->index(sourceRow, 12, sourceParent)).toInt());
    }

    if (!tags.isEmpty()) {
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), Qt::UserRole + 1)
                  .toString().contains(QRegExp(tags.join("|")));
    }

    if (!types.isEmpty())
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 2, sourceParent)).toString().contains(QRegExp(types.join("|")));

    if (markingDay)
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 17, sourceParent))
                  .toString().contains(QRegExp("да|есть"));

    if (nightMarking)
        result &= sourceModel()->data(sourceModel()->index(sourceRow, 20, sourceParent))
                  .toString().contains(QRegExp("да|есть"));

    if (lat > 0 && lon > 0 && radius > 0) {
        double latObstracle = convertCoordInDec(sourceModel()->data(sourceModel()->index(sourceRow, 6, sourceParent)).toString());
        double lonObstracle = convertCoordInDec(sourceModel()->data(sourceModel()->index(sourceRow, 7, sourceParent)).toString());

        if (latObstracle > 0 && lonObstracle > 0) {
            // 6371 - radius Earth
            double d = 6371 * 2 * qAsin(qSqrt(qPow(qSin(qDegreesToRadians((latObstracle - lat) / 2)), 2) +
                                                 qCos(qDegreesToRadians(lat)) *
                                                 qCos(qDegreesToRadians(latObstracle)) *
                                                 qPow(qSin(qDegreesToRadians(qAbs(lonObstracle - lon) / 2)), 2)));
            if (d <= radius)
                result &= true;
            else
                result &= false;
        }
    }
    return result;
}

bool SortSearchFilterObstracleModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
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

void SortSearchFilterObstracleModel::setFilterProperty(QString objectName, QVariant value)
{
    if (objectName.contains("markingDayCheckBox", Qt::CaseInsensitive))
        markingDay = value.toBool();
    else if (objectName.contains("nightMarkingCheckBox", Qt::CaseInsensitive))
        nightMarking = value.toBool();
    else if (objectName.contains("tag", Qt::CaseInsensitive))
        tags = value.toStringList();
    else if (objectName.contains("types", Qt::CaseInsensitive))
        types = value.toStringList();
    else {
        fromHeight = value.toList().at(0).toInt();
        toHeight = value.toList().at(1).toInt();
    }

    invalidateFilter();
}

void SortSearchFilterObstracleModel::setFilterRadius(float lat, float lon, int radius)
{
    this->lat = lat;
    this->lon = lon;
    this->radius = radius;

    invalidateFilter();
}

float SortSearchFilterObstracleModel::convertCoordInDec(const QString coordStr) const
{
    QRegExp regExp("\\D?(\\d{2}|0\\d{2})\\s?(\\d{2})\\s?(\\d{1,2}[\\.\\,]?\\d*)");
    float coordDec = 0;

    if (regExp.indexIn(coordStr) != -1)
        coordDec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).toFloat() / 3600;

    return coordDec;
}
