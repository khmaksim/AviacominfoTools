#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "obstraclesform.h"
#include <QSqlDatabase>
#include <QFile>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ObstraclesForm *form = new ObstraclesForm(this);
    ui->stackedWidget->addWidget(form);

    connectDatabase();

    connect(ui->obstracleButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::showObstracles()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWidget::connectDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    if (!QFile("AviacominfoTools.db").exists())
        qDebug() << "File database is not found.";

    db.setDatabaseName("AviacominfoTools.db");

    if (!db.open())
        qDebug() << "Can not connected to database.";
}
