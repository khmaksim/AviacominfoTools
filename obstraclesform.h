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
//        WaitingSpinnerWidget *spinner;
        QFrame *filterPanel;

        void readSettings();
        void writeSettings();

    private slots:
        void setListView(QVector<Airfield>&);
        void setTableView(QVector<QVector<QString>>&);
//        void getObstracleForAirfield(const QModelIndex&);
        void enabledToolButton();
        void exportToFile();
        void searchAirfield(const QString&);
        void showFilterPanel();
};

#endif // OBSTRACLESFORM_H
