#include "sidebar.h"
#include "ui_sidebar.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
}

SideBar::~SideBar()
{
    delete ui;
}

QSize SideBar::sizeHint()
{
    return QSize(150, 400);
}

void SideBar::checkBoxChanged(bool f)
{
    emit toggled(sender()->objectName(), f);
}
