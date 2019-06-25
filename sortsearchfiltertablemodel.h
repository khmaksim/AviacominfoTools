#ifndef SORTSEARCHMODEL_H
#define SORTSEARCHMODEL_H

#include <QSortFilterProxyModel>

class SortSearchFilterTableModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit SortSearchFilterTableModel(QObject *parent = nullptr);
        ~SortSearchFilterTableModel();

    public slots:
        void setFilterProperty(QString, bool);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    private:
        bool markingDay;
        bool nightMarking;
};

#endif // SORTSEARCHMODEL_H
