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
#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>
#include <QDateTime>
#include "listitemdelegate.h"
#include "searchmodel.h"
#include "waitingspinnerwidget.h"
#include "qgroupheaderview.h"
#include "filterpanel.h"
#include "databaseaccess.h"
#include "sidebar.h"
#include "sortsearchfilterobstraclemodel.h"
#include "checkboxitemdelegate.h"
#include "obstraclestyleditemdelegate.h"
#include "mapview.h"
#include "helper.h"
#include "settingsobstraclesdialog.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

ObstraclesForm::ObstraclesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObstraclesForm)
{
    ui->setupUi(this);
    exportButton = new QToolButton(this);
    exportButton->setEnabled(false);
    exportButton->setText(tr("Export"));
    exportButton->setIconSize(QSize(32, 32));
    exportButton->setIcon(QIcon(":/images/res/img/icons8-save-48.png"));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    displayOnMapButton = new QToolButton(this);
    displayOnMapButton->setEnabled(false);
    displayOnMapButton->setText(tr("Unhide"));
    displayOnMapButton->setIconSize(QSize(32, 32));
    displayOnMapButton->setIcon(QIcon(":/images/res/img/icons8-map-marker-48.png"));
    displayOnMapButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton = new QToolButton(this);
    settingsButton->setText(tr("Update setup"));
    settingsButton->setIconSize(QSize(32, 32));
    settingsButton->setIcon(QIcon(":/images/res/img/icons8-settings-48.png"));
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    updateButton = new QToolButton(this);
    updateButton->setText(tr("Forced update"));
    updateButton->setIconSize(QSize(32, 32));
    updateButton->setIcon(QIcon(":/images/res/img/icons8-downloading-updates-48.png"));
    updateButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exportXlsxButton = new QToolButton(this);
    exportXlsxButton->setEnabled(false);
    exportXlsxButton->setText(tr("Export to Excel"));
    exportXlsxButton->setIconSize(QSize(32, 32));
    exportXlsxButton->setIcon(QIcon(":/images/res/img/icons8-microsoft-excel-48.png"));
    exportXlsxButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mapView = nullptr;
    toolBar = new QToolBar(this);
    toolBar->addWidget(exportButton);
    toolBar->addWidget(displayOnMapButton);
    toolBar->addWidget(settingsButton);
    toolBar->addWidget(updateButton);
    toolBar->addWidget(exportXlsxButton);

    sideBar = new SideBar(this);
    totalObstraclesLabel = new QLabel(tr("Total obstracles: 0"), this);
    selectedObstraclesLabel = new QLabel(tr("Selected obstacles: 0"), this);
    dateUpdatedLabel = new QLabel(this);
    qobject_cast<QMainWindow*>(parent)->statusBar()->addWidget(totalObstraclesLabel, 1);
    qobject_cast<QMainWindow*>(parent)->statusBar()->addWidget(selectedObstraclesLabel, 1);
    qobject_cast<QMainWindow*>(parent)->statusBar()->addWidget(dateUpdatedLabel, 1);

    ui->splitter->setSizes(QList<int>() << 150 << 300);
    ui->gridLayout_2->addWidget(toolBar, 0, 0, 1, 2);
    ui->gridLayout_2->addWidget(ui->splitter, 1, 0, 1, 2);

    airfieldsModel = new QStandardItemModel(this);
    searchAirfieldsModel = new SearchModel(this);
    searchAirfieldsModel->setSourceModel(airfieldsModel);
    searchAirfieldsModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->listView->setItemDelegate(new ListItemDelegate());
    ui->listView->setModel(searchAirfieldsModel);

//    ui->listView->setStyleSheet("QListView::item:selected { background: #66b3ff;color: white; }"
//                                "QListView::item:hover { background: #e6f2ff;color: black; }");

    obstraclesModel = new QStandardItemModel(this);
    sortSearchFilterObstracleModel = new SortSearchFilterObstracleModel(this);
    sortSearchFilterObstracleModel->setSourceModel(obstraclesModel);
    ui->tableView->setModel(sortSearchFilterObstracleModel);

    QGroupHeaderView *groupHeaderView = new QGroupHeaderView(Qt::Horizontal, ui->tableView);
//    groupHeaderView->setStyleSheet("QHeaderView::section { color: black;border: 0.5px solid #bfbfbf; }");
    groupHeaderView->setCheckable(true);
    obstraclesModel->setHorizontalHeaderLabels(QStringList() << tr("*")
                                                             << tr("ID")
                                                             << tr("Name")
                                                             << tr("Type of\n configuration")
                                                             << tr("Human\n settlement")
                                                             << tr("Location options | coordinate\n system")
                                                             << tr("Location options | latitude")
                                                             << tr("Location options | longitude")
                                                             << tr("Location options | latitude of\n center of\n arc/circle")
                                                             << tr("Location options | longitude of\n center of\n arc/circle")
                                                             << tr("Location options | arc/circle\n radius (m)")
                                                             << tr("Location options | horizontal\n accuracy (m)")
                                                             << tr("Height | orthometric\n height MSL (m)")
                                                             << tr("Height | relative\n height AGL (m)")
                                                             << tr("Height | vertical\n accuracy (m)")
                                                             << tr("Design parameters | type of\n material")
                                                             << tr("Design parameters | fragility")
                                                             << tr("Marking day | Yes/no")
                                                             << tr("Marking day | template")
                                                             << tr("Marking day | color")
                                                             << tr("Night marking | Yes/no")
                                                             << tr("Night marking | color")
                                                             << tr("Night marking | type of\n light")
                                                             << tr("Night marking | intensity")
                                                             << tr("Night marking | lights working\n time")
                                                             << tr("Night marking | compliance\n 14 ADJ. ICAO")
                                                             << tr("Data source | supplier")
                                                             << tr("Data source | date of\n submission"));

    ui->tableView->setHorizontalHeader(groupHeaderView);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView->horizontalHeader()->setSectionsClickable(true);
    ui->tableView->setItemDelegateForColumn(0, new CheckboxItemDelegate(this));
    ui->tableView->setItemDelegate(new ObstracleStyledItemDelegate(this));

    updateModelAirfields();

//    if (airfieldsModel->rowCount() == 0) {
    spinner = new WaitingSpinnerWidget(this);
    spinner->setRoundness(70.0);
    spinner->setMinimumTrailOpacity(15.0);
    spinner->setTrailFadePercentage(70.0);
    spinner->setNumberOfLines(12);
    spinner->setLineLength(15);
    spinner->setLineWidth(3);
    spinner->setInnerRadius(10);
    spinner->setRevolutionsPerSecond(1);
    spinner->setColor(QColor(93, 93, 93));
    connect(airfieldsModel, SIGNAL(modelReset()), spinner, SLOT(stop()));
    //    }

    readSettings();

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), sideBar, SLOT(resetFilter()));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(updateModelObstracles(QModelIndex)));
    connect(obstraclesModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(enabledToolButton()));
    connect(obstraclesModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(updateStatusSelectedObstracles()));
    connect(sortSearchFilterObstracleModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateStatusSelectedObstracles()));
    connect(sortSearchFilterObstracleModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateStatusSelectedObstracles()));
    connect(sortSearchFilterObstracleModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(setCheckedAllRowTable()));
    connect(sortSearchFilterObstracleModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(setCheckedAllRowTable()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(exportToFile()));
    connect(displayOnMapButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
    connect(settingsButton, SIGNAL(clicked(bool)), this, SLOT(showSettings()));
    connect(updateButton, SIGNAL(clicked(bool)), SIGNAL(updated()));
    connect(updateButton, SIGNAL(clicked(bool)), spinner, SLOT(start()));
    connect(exportXlsxButton, SIGNAL(clicked(bool)), this, SLOT(exportToXLSXFile()));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), searchAirfieldsModel, SLOT(setFilterRegExp(QString)));
    connect(sideBar, SIGNAL(searchTextChanged(QString)), sortSearchFilterObstracleModel, SLOT(setFilterRegExp(QString)));
    connect(sideBar, SIGNAL(changedFilterProperty(QString, QVariant)), sortSearchFilterObstracleModel, SLOT(setFilterProperty(QString, QVariant)));
    connect(sideBar, SIGNAL(filterRadius()), this, SLOT(setFilterRadius()));
    connect(sideBar, SIGNAL(displayObstracles(QVariant, QVariant)), this, SLOT(showObstracles(QVariant, QVariant)));
    connect(groupHeaderView, SIGNAL(clickedCheckBox(bool)), this, SLOT(setCheckedAllRowTable(bool)));
    connect(DatabaseAccess::getInstance(), SIGNAL(updatedTags()), this, SLOT(updateModelObstracles()));
}

ObstraclesForm::~ObstraclesForm()
{
    writeSettings();
    if (mapView != nullptr) {
        mapView->close();
        delete mapView;
    }
    delete ui;
}

void ObstraclesForm::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
//    sideBar->setGeometry(width() - 20, toolBar->height(), sideBar->size().width(), height() - toolBar->height());

    sideBar->setGeometry(0 - sideBar->size().width() + 20,
                         toolBar->height() + ui->searchLineEdit->height() + ui->listView->sizeHintForRow(0),
                         sideBar->size().width(),
                         height() - toolBar->height() - ui->searchLineEdit->height() - ui->listView->sizeHintForRow(0));
    sideBar->reset();
}

void ObstraclesForm::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue(ui->splitter->objectName(), ui->splitter->saveState());
    settings.setValue(ui->tableView->objectName(), ui->tableView->horizontalHeader()->saveState());
    settings.endGroup();
}

void ObstraclesForm::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    ui->splitter->restoreState(settings.value(ui->splitter->objectName()).toByteArray());
    ui->tableView->horizontalHeader()->restoreState(settings.value(ui->tableView->objectName()).toByteArray());
    settings.endGroup();
    settings.beginGroup("database");
    dateUpdatedLabel->setText(QString(tr("Date updated: %1").arg(settings.value("datetime_updated").toDateTime().toString("dd.MM.yyyy"))));
    settings.endGroup();
}

void ObstraclesForm::updateModelAirfields()
{
    QVector<QVariantList> airfields = DatabaseAccess::getInstance()->getAirfields();

    // remove all rows
    airfieldsModel->clear();

    for (QVector<QVariantList>::iterator it = airfields.begin(); it != airfields.end(); ++it) {
        QStandardItem *item = new QStandardItem;
        item->setData((*it).at(0).toString(), ListItemDelegate::NameAirfieldRole);
        item->setData((*it).at(1).toString(), ListItemDelegate::CodeICAORole);
        item->setData((*it).at(2).toUInt(), ListItemDelegate::IdRole);
        airfieldsModel->appendRow(item);
    }

    ui->listView->repaint();
}

void ObstraclesForm::updateModelObstracles(const QModelIndex &index)
{
    uint idAirfield = 0;
    if (index.isValid())
        idAirfield = searchAirfieldsModel->data(index, ListItemDelegate::IdRole).toUInt();
    else
        idAirfield = searchAirfieldsModel->data(ui->listView->currentIndex(), ListItemDelegate::IdRole).toUInt();

    QVector<QVariantList> obstracles = DatabaseAccess::getInstance()->getObstracles(idAirfield);
    // uncheked header
    qobject_cast<QGroupHeaderView*>(ui->tableView->horizontalHeader())->setChecked(false);
    // remove all rows
    while (obstraclesModel->rowCount() > 0) {
        obstraclesModel->removeRow(0);
    }

    QStringList typesObstracle = QStringList();
    for (int i = 0; i < obstracles.size(); i++) {
        QList<QStandardItem *> items;
        QVariantList fields = obstracles.at(i);

        QStandardItem *item = new QStandardItem();
        item->setData(fields.takeLast().toString(), Qt::UserRole + 1);      // set tags for obstracles to first column
        item->setData(fields.takeLast().toString(), Qt::UserRole + 2);      // set datetime last updated
        items.append(item);
        // get name type obstracles
        for (int j = 0; j < fields.size(); j++) {
            if (j == 1)
                typesObstracle << fields.at(j).toString();
            items.append(new QStandardItem(fields.at(j).toString()));
        }
        obstraclesModel->appendRow(items);
    }
    // set list type obstracles for filter in side bar
    typesObstracle.removeDuplicates();
    sideBar->updateTypeObstracleFilter(typesObstracle);

    // update number obstracle in model for status bar
    totalObstraclesLabel->setText(totalObstraclesLabel->text().replace(QRegExp("\\d+"), QString::number(sortSearchFilterObstracleModel->rowCount())));
}

void ObstraclesForm::enabledToolButton()
{
    bool isEnable = false;
    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++) {
        if (sortSearchFilterObstracleModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            isEnable = true;
            break;
        }
    }
    exportButton->setEnabled(isEnable);
    displayOnMapButton->setEnabled(isEnable);
    exportXlsxButton->setEnabled(isEnable);
    return;
}

void ObstraclesForm::exportToFile()
{
    QString nameFile = QFileDialog::getSaveFileName(this, tr("Save file"), QString("C:/prep.txt"));
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
    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++) {
        if (sortSearchFilterObstracleModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            out << sortSearchFilterObstracleModel->index(row, 6).data().toString().replace("с", "N").replace("ю", "S").remove(QRegExp("[\\s\\.]")).append("0") << endl;
            out << sortSearchFilterObstracleModel->index(row, 7).data().toString().replace("в", "E").replace("з", "W").remove(QRegExp("[\\s\\.]")).append("0") << endl;
            out << sortSearchFilterObstracleModel->index(row, 12).data().toString() << endl;
            if (sortSearchFilterObstracleModel->index(row, 2).data(Qt::DisplayRole).toString().contains("Естественное препятствие"))
                out << "2" << endl;
            else
                out << (sortSearchFilterObstracleModel->index(row, 20).data().toString().contains(QRegExp("(да|есть)")) ? "1" : "0") << endl;
//            out << endl;
            out << "1" << endl;
        }
    }
    file.commit();
}

void ObstraclesForm::exportToXLSXFile()
{
    QString nameFile = QFileDialog::getSaveFileName(this, tr("Export to XLSX file"), QDir::currentPath(), tr("Excel format (*.xlsx)"));
    if (nameFile.isEmpty()) {
        qDebug() << "Empty name export file";
        return;
    }

    Document doc;

    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++) {
        if (sortSearchFilterObstracleModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            for (int col = 1; col < sortSearchFilterObstracleModel->columnCount(); col++) {
                QVariant writeValue = sortSearchFilterObstracleModel->index(row, col).data().toString();
                doc.write(row + 1, col, writeValue);
            }
        }
    }
    doc.saveAs(nameFile);
}
//void ObstraclesForm::showFilterPanel()
//{
//    FilterPanel *filterPanel = new FilterPanel(this);
//    QPoint pos = ui->tableView->mapToGlobal(QPoint(0, 0));
//    int widthTableView = ui->tableView->geometry().width();
//    int heightTableView = ui->tableView->geometry().height();

//    filterPanel->setGeometry(pos.x() + widthTableView / 2, pos.y() + heightTableView / 2,
//                             filterPanel->width(), filterPanel->height());
//    filterPanel->show();
//}

void ObstraclesForm::setCheckedAllRowTable(bool checked)
{
    if (sender() != qobject_cast<QGroupHeaderView*>(ui->tableView->horizontalHeader())) {
        qobject_cast<QGroupHeaderView*>(ui->tableView->horizontalHeader())->setChecked(checked);
    }
    if (sortSearchFilterObstracleModel->rowCount() == 0)
        return;
    sortSearchFilterObstracleModel->setDynamicSortFilter(false);
    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++)
        sortSearchFilterObstracleModel->setData(sortSearchFilterObstracleModel->index(row, 0), checked, Qt::CheckStateRole);
}

void ObstraclesForm::setFilterRadius()
{
    sortSearchFilterObstracleModel->setFilterRadius(sideBar->getLat(), sideBar->getLon(), sideBar->getRadius());
}

QVariantList ObstraclesForm::getCheckedObstralcles()
{
    QVariantList idSelectedObstracles;

    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++) {
        if (sortSearchFilterObstracleModel->index(row, 0).data(Qt::CheckStateRole).toBool())
            idSelectedObstracles.append(sortSearchFilterObstracleModel->index(row, 1).data(Qt::DisplayRole).toString());
    }
    return idSelectedObstracles;
}

void ObstraclesForm::showObstracles(QVariant coordinate, QVariant radius)
{
    if (mapView == nullptr) {
        mapView = new MapView;
        connect(mapView, SIGNAL(checked(bool, QString)), this, SLOT(setChecked(bool, QString)));
    }
    mapView->clearMap();

    QPointF centerMap = coordinate.toPointF();
    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++) {
        if (sortSearchFilterObstracleModel->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            ObstraclePoint obstraclePoint;
            obstraclePoint.lat = Helper::convertCoordinateInDec(sortSearchFilterObstracleModel->index(row, 6).data(Qt::DisplayRole).toString());
            obstraclePoint.lon = Helper::convertCoordinateInDec(sortSearchFilterObstracleModel->index(row, 7).data(Qt::DisplayRole).toString());
            obstraclePoint.height = sortSearchFilterObstracleModel->index(row, 12).data(Qt::DisplayRole).toString();
            obstraclePoint.type = sortSearchFilterObstracleModel->index(row, 2).data(Qt::DisplayRole).toString().contains("Естественное препятствие");
            obstraclePoint.marker = (sortSearchFilterObstracleModel->index(row, 17).data(Qt::DisplayRole).toString().contains(QRegExp("да|есть")) ||
                                     sortSearchFilterObstracleModel->index(row, 20).data(Qt::DisplayRole).toString().contains(QRegExp("да|есть")));
            obstraclePoint.id = sortSearchFilterObstracleModel->index(row, 1).data(Qt::DisplayRole).toString();
            mapView->addObstracle(obstraclePoint);

            if (coordinate.toPointF().isNull()) {
                centerMap = QPointF(obstraclePoint.lat, obstraclePoint.lon);
            }
        }
    }
    setCheckedAllRowTable();
    mapView->setCenter(centerMap);
    mapView->setRadius(radius);
    if (centerMap.isNull()) {
        QMessageBox::warning(this, tr("Warning"), tr("You must select the obstacles displayed in the table!"));
        return;
    }
    mapView->show();
}

void ObstraclesForm::showUpdated()
{
    updateModelAirfields();

    dateUpdatedLabel->setText(QString(tr("Date updated: %1")).arg(QDate::currentDate().toString("dd.MM.yyyy")));
    QMessageBox::information(this, tr("Information"), tr("Obstacle database updated!"));
}

void ObstraclesForm::updateStatusSelectedObstracles()
{
    int numSelected = getCheckedObstralcles().size();
    if (numSelected >= 0)
        selectedObstraclesLabel->setText(selectedObstraclesLabel->text().replace(QRegExp("\\d+"), QString::number(numSelected)));
}

void ObstraclesForm::setChecked(bool checked, QString id)
{
    for (int row = 0; row < sortSearchFilterObstracleModel->rowCount(); row++)
        if (sortSearchFilterObstracleModel->index(row, 1).data(Qt::DisplayRole).toString().contains(id)) {
            sortSearchFilterObstracleModel->setData(sortSearchFilterObstracleModel->index(row, 0), checked, Qt::CheckStateRole);
        }
}

void ObstraclesForm::showSettings()
{
    SettingsObstraclesDialog settingsObstraclesDialog(this);
    settingsObstraclesDialog.exec();
}
