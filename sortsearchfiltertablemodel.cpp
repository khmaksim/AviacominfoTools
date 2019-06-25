#include "sortsearchfiltertablemodel.h"
#include "listitemdelegate.h"

SortSearchFilterTableModel::SortSearchFilterTableModel(QObject *parent)
{
    Q_UNUSED(parent)
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

    return result;
}
