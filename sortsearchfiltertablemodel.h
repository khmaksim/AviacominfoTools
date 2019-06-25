#ifndef SORTSEARCHMODEL_H
#define SORTSEARCHMODEL_H

#include <QSortFilterProxyModel>

class SortSearchFilterTableModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit SortSearchFilterTableModel(QObject *parent = nullptr);
        ~SortSearchFilterTableModel();

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // SORTSEARCHMODEL_H
