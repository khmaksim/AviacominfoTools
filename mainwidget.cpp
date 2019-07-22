#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "obstraclesform.h"
#include "obstracleshandler.h"
#include "databaseaccess.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    obstraclesHandler = new ObstraclesHandler(this);

    readSettings();

    ObstraclesForm *form = new ObstraclesForm(this);
    ui->stackedWidget->addWidget(form);

    ui->stackedWidget->setCurrentIndex(1);

    connect(obstraclesHandler, SIGNAL(updated()), form, SLOT(showUpdated()));
//    connect(obstraclesHandler, SIGNAL(finished(Airfield,QVector<QVector<QString> >&)), DatabaseAccess::getInstance(), SLOT(update(Airfield,QVector<QVector<QString> >&)));
    connect(ui->obstracleButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
}

MainWidget::~MainWidget()
{
    writeSettings();
    delete ui;
}

void MainWidget::showObstracles()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWidget::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue("maximized", this->isMaximized());
    settings.endGroup();
}

void MainWidget::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    if (settings.value("maximized").toBool())
        this->showMaximized();
    settings.endGroup();
    settings.beginGroup("database");
    if (settings.value("datetime_updated").toDateTime().addDays(1) < QDateTime::currentDateTime()) {
        settings.setValue("datetime_updated", QDateTime::currentDateTime());
        obstraclesHandler->checkUpdates();
    }
    settings.endGroup();
}
