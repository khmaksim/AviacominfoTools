#ifndef SORTSEARCHMODEL_H
#define SORTSEARCHMODEL_H

#include <QSortFilterProxyModel>

class SortSearchFilterTableModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit SortSearchFilterTableModel(QObject *parent = nullptr);
        ~SortSearchFilterTableModel();

        void setFilterRadius(float, float, int);

    public slots:
        void setFilterProperty(QString, QVariant);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
        bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

    private:
        QStringList tags;
        QStringList types;
        int fromHeight;
        int toHeight;
        bool markingDay;
        bool nightMarking;
        float lat;
        float lon;
        int radius;

        float convertCoordInDec(const QString) const;
};

#endif // SORTSEARCHMODEL_H
