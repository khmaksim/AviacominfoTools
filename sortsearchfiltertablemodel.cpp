#include "sortsearchfiltertablemodel.h"
#include "listitemdelegate.h"
#include <QDebug>

SortSearchFilterTableModel::SortSearchFilterTableModel(QObject *parent)
{
    Q_UNUSED(parent)
    markingDay = false;
    nightMarking = false;
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

    return result;
}

void SortSearchFilterTableModel::setFilterProperty(QString objectName, bool flag)
{
    if (objectName.contains("day", Qt::CaseInsensitive))
        markingDay = flag;
    else if (objectName.contains("night", Qt::CaseInsensitive))
        nightMarking = flag;

    invalidateFilter();
}
