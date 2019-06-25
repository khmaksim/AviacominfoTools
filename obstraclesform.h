#ifndef OBSTRACLESFORM_H
#define OBSTRACLESFORM_H

#include <QWidget>
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
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = 0);
        ~ObstraclesForm();

//    public slots:
//        void updateData();

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *filterButton;
        QStandardItemModel *airfieldsModel;
        TableModel *obstraclesModel;
        SearchModel *searchModel;
        SortSearchFilterTableModel *sortSearchFilterTableModel;
//        WaitingSpinnerWidget *spinner;
        QFrame *filterPanel;
        SideBar *sideBar;

        void readSettings();
        void writeSettings();
        void setListView();
        void setTableView(QVector<QVariantList>);

    private slots:
        void getObstracleForAirfield(const QModelIndex&);
        void enabledToolButton();
        void exportToFile();
//        void searchAirfield(const QString&);
        void showFilterPanel();
};

#endif // OBSTRACLESFORM_H
