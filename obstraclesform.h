#ifndef OBSTRACLESFORM_H
#define OBSTRACLESFORM_H

#include <QWidget>
#include "obstracleshandler.h"

namespace Ui {
    class ObstraclesForm;
}

class QStandardItemModel;
class ObstraclesHandler;
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = 0);
        ~ObstraclesForm();

    private:
        Ui::ObstraclesForm *ui;
        ObstraclesHandler *obstraclesHandler;
        QStandardItemModel *airfieldsModel;

    private slots:
        void setListView(QVector<Airfield>&);
        void getObstracleAirfield(const QModelIndex&);
};

#endif // OBSTRACLESFORM_H
