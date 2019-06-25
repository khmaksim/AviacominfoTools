#include "sidebar.h"
#include "ui_sidebar.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
}

SideBar::~SideBar()
{
    delete ui;
}

QSize SideBar::sizeHint()
{
    return QSize(150, 400);
}
