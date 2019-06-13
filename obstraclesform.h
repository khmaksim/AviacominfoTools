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
class ObstraclesHandler;
class TableModel;
class SearchModel;
class WaitingSpinnerWidget;
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = 0);
        ~ObstraclesForm();

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        ObstraclesHandler *obstraclesHandler;
        QStandardItemModel *airfieldsModel;
        TableModel *obstraclesModel;
        SearchModel *searchModel;
        WaitingSpinnerWidget *spinner;

    private slots:
        void setListView(QVector<Airfield>&);
        void setTableView(QVector<QVector<QString>>&);
        void getObstracleAirfield(const QModelIndex&);
        void enabledToolButton();
        void exportToFile();
        void searchAirfield(const QString&);
};

#endif // OBSTRACLESFORM_H
