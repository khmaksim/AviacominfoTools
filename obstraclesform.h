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

    public slots:
        void showUpdated();

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *displayOnMapButton;
        QToolButton *settingsButton;
        QToolButton *updateButton;
        QToolButton *exportXlsxButton;
        QToolButton *exportDbXlsxButton;
        QStandardItemModel *airfieldsModel;
        QStandardItemModel *obstraclesModel;
        SearchModel *searchAirfieldsModel;
        SortSearchFilterObstracleModel *sortSearchFilterObstracleModel;
        QFrame *filterPanel;
        SideBar *sideBar;
        MapView *mapView;
        QLabel *totalObstraclesLabel;
        QLabel *selectedObstraclesLabel;
        QLabel *dateUpdatedLabel;
        bool isConvert;
        QByteArray headerTableState;

        void readSettings();
        void writeSettings();
        void clearTable();
        void setConfigTable();

    private slots:
        void updateModelAirfields();
        void updateModelObstracles(const QModelIndex &index = QModelIndex());
        void enabledToolButton();
        void exportToFile();
        void exportToXLSXFile();
//        void showFilterPanel();
        void setCheckedAllRowTable(bool checked = false);
        void setChecked(bool, QString);
        void setFilterRadius();
        void showObstracles(QVariant c = QVariant(), QVariant radius = QVariant(0));
        void updateStatusSelectedObstracles();
        void showSettingsDialog();
        void showConfirmMessage();

    signals:
        void update();
};

#endif // OBSTRACLESFORM_H
