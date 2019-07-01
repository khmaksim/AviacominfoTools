#include "sidebar.h"
#include "ui_sidebar.h"
#include <QDebug>

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    ui->minRadiusLabel->setNum(ui->radiusSlider->minimum());
    ui->maxRadiusLabel->setNum(ui->radiusSlider->maximum());
    setValueRadius(ui->radiusSlider->value());

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(setValueRadius(int)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SIGNAL(filterRadius()));
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

void SideBar::setValueRadius(int value)
{
    ui->valueRadiusLabel->setText(QString::number(value).append(tr(" km")));
}

float SideBar::getLat()
{
    return convertCoordInDec(ui->latLineEdit->text());
}

float SideBar::getLon()
{
    return convertCoordInDec(ui->lonLineEdit->text());
}

int SideBar::getRadius()
{
    return ui->radiusSlider->value();
}

float SideBar::convertCoordInDec(QString coordStr)
{
    QRegExp regExp("\\S(\\d{1,3})\\s(\\d{1,2})\\s([\\d\\.]+)");
    float coordDec = 0;

    if (regExp.indexIn(coordStr) != -1) {
        coordDec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).toFloat() / 3600;
    }

    return coordDec;
}
