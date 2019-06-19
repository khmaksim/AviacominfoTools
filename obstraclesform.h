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
        QStandardItemModel *airfieldsModel;
        TableModel *obstraclesModel;
        SearchModel *searchModel;
        WaitingSpinnerWidget *spinner;

        void readSettings();
        void writeSettings();

    private slots:
        void setListView(QVector<Airfield>&);
        void setTableView(QVector<QVector<QString>>&);
//        void getObstracleForAirfield(const QModelIndex&);
        void enabledToolButton();
        void exportToFile();
        void searchAirfield(const QString&);
};

#endif // OBSTRACLESFORM_H
