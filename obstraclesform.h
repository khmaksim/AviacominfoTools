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
        TableModel *obstraclesModel;
        SearchModel *searchAirfieldsModel;
        SortSearchFilterTableModel *sortSearchFilterTableModel;
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
        void setCheckedAllRowTable(bool);
        void setFilterRadius();
//        void showTags(const QModelIndex&);
        void showObstracles(QVariant c = QVariant());
        void updateStatusSelectedObstracles();
};

#endif // OBSTRACLESFORM_H
