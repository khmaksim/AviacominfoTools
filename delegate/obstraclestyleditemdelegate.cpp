#include "obstraclestyleditemdelegate.h"
#include <QDebug>
#include <QDateTime>
#include <QPainter>
#include <QSettings>

#define AIRAC_CYCLE 28

void ObstracleStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPainter *p(painter);
    QStyleOptionViewItem opt(option);
    QSettings settings;

    settings.beginGroup("database");
    QDateTime lastAutoUpdated = settings.value("datetime_updated").toDateTime();
    settings.endGroup();

    QDate startCycle(2020, 1, 2);     // дата начала AIRAC цикла в 2021 году
    // date update for entries in database
    QDateTime lastUpdatedEntries = QDateTime::fromString(index.model()->data(index.model()->index(index.row(), 0), Qt::UserRole + 2).toString(), "yyyy-MM-dd HH:mm:ss");
    // data update AIP
    QString dateUpdatedSource = index.model()->index(index.row(), index.model()->columnCount() - 1).data().toString();

    int yearUpdated = dateUpdatedSource.left(2).prepend("20").toInt();
    int countPastYears = yearUpdated - startCycle.year();
    QDate startCycleCurrentYear(startCycle.addDays(countPastYears * 13 * AIRAC_CYCLE));

    if (startCycleCurrentYear.year() < yearUpdated)
        startCycleCurrentYear = startCycleCurrentYear.addDays(AIRAC_CYCLE);
    int numCycle = dateUpdatedSource.right(2).toInt();

    QDate dateLastUpdated = startCycleCurrentYear.addDays(AIRAC_CYCLE * (numCycle - 1));

    // new data next cycle
    if (dateLastUpdated.addDays(AIRAC_CYCLE) > lastUpdatedEntries.date()) {
        p->fillRect(opt.rect, QBrush(QColor(148, 243, 159, 64), Qt::SolidPattern));
    }
    if (lastAutoUpdated.date() != lastUpdatedEntries.date()) {
        p->fillRect(opt.rect, QBrush(QColor(255, 70, 94, 64), Qt::SolidPattern));
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
