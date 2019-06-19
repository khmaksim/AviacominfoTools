#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "obstraclesform.h"
#include "obstracleshandler.h"
#include "database.h"
#include <QDebug>
#include <QSettings>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ObstraclesForm *form = new ObstraclesForm(this);
    ui->stackedWidget->addWidget(form);

    db = new Database(this);
    obstraclesHandler = new ObstraclesHandler(this);

    readSettings();

    connect(obstraclesHandler, SIGNAL(finished(Airfield,QVector<QVector<QString> >&)), db, SLOT(update(Airfield,QVector<QVector<QString> >&)));
    connect(ui->obstracleButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), obstraclesHandler, SLOT(checkUpdates()));
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
}
