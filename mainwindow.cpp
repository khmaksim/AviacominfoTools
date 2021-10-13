#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obstraclesform.h"
#include "obstracleshandler.h"
#include "databaseaccess.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include "waitingspinnerwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create and settings spinner
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

    obstraclesHandler = new ObstraclesHandler(this);

    ObstraclesForm *form = new ObstraclesForm(this);
    ui->stackedWidget->addWidget(form);
    ui->stackedWidget->setCurrentIndex(1);

    connect(obstraclesHandler, SIGNAL(updated()), spinner, SLOT(stop()));
    connect(obstraclesHandler, SIGNAL(updated()), form, SLOT(showUpdated()));
    connect(obstraclesHandler, SIGNAL(updated()), form, SLOT(updateModelAirfields()));
    connect(form, SIGNAL(update()), spinner, SLOT(start()));
    connect(form, SIGNAL(update()), obstraclesHandler, SLOT(update()));
//    connect(obstraclesHandler, SIGNAL(finished(Airfield,QVector<QVector<QString> >&)), DatabaseAccess::getInstance(), SLOT(update(Airfield,QVector<QVector<QString> >&)));
    connect(ui->obstracleButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::showObstracles()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue("maximized", this->isMaximized());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    if (settings.value("maximized").toBool())
        this->showMaximized();
    settings.endGroup();
    settings.beginGroup("database");
    int updateRate = settings.value("update_rate", 1).toInt();
    if (settings.value("datetime_updated").toDateTime().addDays(updateRate) < QDateTime::currentDateTime()) {
        obstraclesHandler->update();
        spinner->start();
    }
    settings.endGroup();
}
