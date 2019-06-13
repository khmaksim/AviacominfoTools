#include "obstraclesform.h"
#include "ui_obstraclesform.h"
#include "obstracleshandler.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>
#include <QSaveFile>
#include <QSortFilterProxyModel>
#include "listitemdelegate.h"
#include "tablemodel.h"
#include "searchmodel.h"
#include "waitingspinnerwidget.h"

ObstraclesForm::ObstraclesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObstraclesForm)
{
    ui->setupUi(this);
    exportButton = new QToolButton(this);
    exportButton->setEnabled(false);
    exportButton->setText(tr("Export"));
    exportButton->setIconSize(QSize(32, 32));
    exportButton->setIcon(QIcon(":/images/res/img/filesave.png"));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    toolBar = new QToolBar(this);
    toolBar->addWidget(exportButton);

    ui->splitter->setSizes(QList<int>() << 150 << 300);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(toolBar, 0, 0, 1, 2);
    layout->addWidget(ui->splitter, 1, 0, 2, 2);
    setLayout(layout);

    airfieldsModel = new QStandardItemModel(this);
    searchModel = new SearchModel(this);
    searchModel->setSourceModel(airfieldsModel);
    searchModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    obstraclesModel = new TableModel(this);
    ListItemDelegate *listItemDelegate = new ListItemDelegate();

    ui->listView->setItemDelegate(listItemDelegate);
    ui->listView->setModel(searchModel);
    ui->tableView->setModel(obstraclesModel);

    obstraclesHandler = new ObstraclesHandler;

    spinner = new WaitingSpinnerWidget(this);
    spinner->setRoundness(70.0);
    spinner->setMinimumTrailOpacity(15.0);
    spinner->setTrailFadePercentage(70.0);
    spinner->setNumberOfLines(12);
    spinner->setLineLength(15);
    spinner->setLineWidth(4);
    spinner->setInnerRadius(10);
    spinner->setRevolutionsPerSecond(1);
//    spinner->setColor(QColor(81, 4, 71));
    spinner->start();

    connect(obstraclesHandler, SIGNAL(finished(QVector<Airfield>&)), this, SLOT(setListView(QVector<Airfield>&)));
    connect(obstraclesHandler, SIGNAL(finished(QVector<Airfield>&)), spinner, SLOT(stop()));
    connect(obstraclesHandler, SIGNAL(finished(QVector<QVector<QString>>&)), this, SLOT(setTableView(QVector<QVector<QString>>&)));
    connect(obstraclesHandler, SIGNAL(finished(QVector<QVector<QString>>&)), spinner, SLOT(stop()));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(getObstracleAirfield(QModelIndex)));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), spinner, SLOT(start()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(enabledToolButton()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(exportToFile()));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchAirfield(QString)));
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
    QString hrefFile = searchModel->data(index, ListItemDelegate::HrefRole).toString();

    if (!hrefFile.isEmpty())
        obstraclesHandler->getListObstracles(hrefFile);
}

void ObstraclesForm::setTableView(QVector<QVector<QString>> &dataTable)
{
    obstraclesModel->clear();
    for (int i = 0; i < dataTable.size(); i++) {
        QVector<QString> dataRow = dataTable.at(i);
        QList<QStandardItem *> items;

        QStandardItem *itemCheckable = new QStandardItem();
        itemCheckable->setData(false, Qt::CheckStateRole);
        itemCheckable->setFlags(itemCheckable->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
        items.append(itemCheckable);
        for (int j = 0; j < dataRow.size(); j++) {
            items.append(new QStandardItem(dataRow.at(j)));
        }
        obstraclesModel->appendRow(items);
    }

    ui->tableView->repaint();
}

void ObstraclesForm::enabledToolButton()
{
    for (int row = 0; row < obstraclesModel->rowCount(); row++) {
        if (obstraclesModel->item(row)->data(Qt::CheckStateRole).toBool()) {
            exportButton->setEnabled(true);
            return;
        }
    }
    exportButton->setEnabled(false);
    return;
}

void ObstraclesForm::exportToFile()
{
    QString nameFile = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::homePath());
    if (nameFile.isEmpty()) {
        qDebug() << "Empty name save file";
        return;
    }

    QSaveFile file(nameFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (int row = 0; row < obstraclesModel->rowCount(); row++) {
        if (obstraclesModel->item(row)->data(Qt::CheckStateRole).toBool()) {
            out << obstraclesModel->item(row, 6)->text().replace("с", "N").remove(QRegExp("[\\s\\.]")) << endl;
            out << obstraclesModel->item(row, 7)->text().replace("в", "E").remove(QRegExp("[\\s\\.]")) << endl;
            out << obstraclesModel->item(row, 12)->text() << endl;
            out << endl;
            out << "1" << endl;
        }
    }
    file.commit();
}

void ObstraclesForm::searchAirfield(const QString &searchRequest)
{
    searchModel->setFilterRegExp(searchRequest);
}
