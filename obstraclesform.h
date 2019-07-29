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
class SearchModel;
class WaitingSpinnerWidget;
class QFrame;
class SortSearchFilterObstracleModel;
class SideBar;
class MapView;
class QLabel;
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = 0);
        ~ObstraclesForm();

        QVariantList getCheckedObstralcles();
        WaitingSpinnerWidget *spinner;

    public slots:
        void showUpdated();

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *displayOnMapButton;
        QStandardItemModel *airfieldsModel;
        QStandardItemModel *obstraclesModel;
        SearchModel *searchAirfieldsModel;
        SortSearchFilterObstracleModel *sortSearchFilterObstracleModel;
        QFrame *filterPanel;
        SideBar *sideBar;
        MapView *mapView;
        QLabel *totalObstraclesLabel;
        QLabel *selectedObstraclesLabel;

        void readSettings();
        void writeSettings();
        double parserCoordinate(QString);
        void updateModelAirfields();

    private slots:
        void updateModelObstracles(const QModelIndex &index = QModelIndex());
        void enabledToolButton();
        void exportToFile();
//        void showFilterPanel();
        void setCheckedAllRowTable(bool checked = false);
        void setCheckedOne(QString);
        void setFilterRadius();
        void showObstracles(QVariant c = QVariant(), QVariant radius = QVariant(0));
        void updateStatusSelectedObstracles();
};

#endif // OBSTRACLESFORM_H
