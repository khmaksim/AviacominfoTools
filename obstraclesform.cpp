#include "obstraclesform.h"
#include "ui_obstraclesform.h"
#include "obstracleshandler.h"
#include <QDebug>
#include <QStandardItemModel>
#include "listitemdelegate.h"

ObstraclesForm::ObstraclesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObstraclesForm)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 150 << 300);

    airfieldsModel = new QStandardItemModel(this);
    ListItemDelegate *listItemDelegate = new ListItemDelegate();

    ui->listView->setItemDelegate(listItemDelegate);
    ui->listView->setModel(airfieldsModel);

    obstraclesHandler = new ObstraclesHandler;

    connect(obstraclesHandler, SIGNAL(finished(QVector<Airfield>&)), this, SLOT(setListView(QVector<Airfield>&)));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(getObstracleAirfield(QModelIndex)));
}

ObstraclesForm::~ObstraclesForm()
{
    delete obstraclesHandler;
    delete ui;
}

void ObstraclesForm::setListView(QVector<Airfield> &airfields)
{
    QVector<Airfield>::iterator it;

    for (it = airfields.begin(); it != airfields.end(); ++it) {
        QStandardItem *item = new QStandardItem;
        item->setData((*it).name, ListItemDelegate::NameAirfieldRole);
        item->setData((*it).icao, ListItemDelegate::CodeICAORole);
        item->setData((*it).href, ListItemDelegate::HrefRole);
        airfieldsModel->appendRow(item);
    }

    ui->listView->repaint();
}

void ObstraclesForm::getObstracleAirfield(const QModelIndex &index)
{
    QString hrefFile = airfieldsModel->data(index, ListItemDelegate::HrefRole).toString();

    if (!hrefFile.isEmpty())
        obstraclesHandler->getListObstracles(hrefFile);
}
