#ifndef OBSTRACLESFORM_H
#define OBSTRACLESFORM_H

#include <QWidget>
#include <QModelIndex>
#include "obstracleshandler.h"

namespace Ui {
    class ObstraclesForm;
}

class QStandardItemModel;
class QToolBar;
class QToolButton;
class TableModel;
class SearchModel;
class WaitingSpinnerWidget;
class QFrame;
class SortSearchFilterTableModel;
class SideBar;
class MapView;
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = 0);
        ~ObstraclesForm();

        QVariantList getCheckedObstralcles();

    public slots:
        void updateModelAirfields();

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *filterButton;
        QStandardItemModel *airfieldsModel;
        TableModel *obstraclesModel;
        SearchModel *searchAirfieldsModel;
        SortSearchFilterTableModel *sortSearchFilterTableModel;
        WaitingSpinnerWidget *spinner;
        QFrame *filterPanel;
        SideBar *sideBar;
        MapView *mapView;

        void readSettings();
        void writeSettings();
        double parserCoordinate(QString);

    private slots:
        void updateModelObstracles(const QModelIndex &index = QModelIndex());
        void enabledToolButton();
        void exportToFile();
        void showFilterPanel();
        void setCheckedAllRowTable(bool);
        void setFilterRadius();
        void showTags(const QModelIndex&);
        void showObstracles(QVariant);
};

#endif // OBSTRACLESFORM_H
