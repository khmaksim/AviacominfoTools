#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "obstraclesform.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ObstraclesForm *form = new ObstraclesForm(this);
    ui->stackedWidget->addWidget(form);

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
