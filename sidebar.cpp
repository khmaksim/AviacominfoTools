#include "sidebar.h"
#include "ui_sidebar.h"
#include <QDebug>
#include <QInputDialog>
#include "flowlayout.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    ui->minRadiusLabel->setNum(ui->radiusSlider->minimum());
    ui->maxRadiusLabel->setNum(ui->radiusSlider->maximum());

    FlowLayout *scrollAreaLayout = new FlowLayout();
//    ui->scrollArea->setLayout(scrollAreaLayout);
    ui->scrollAreaWidgetContents->setLayout(scrollAreaLayout);

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValueRadius(int)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SIGNAL(filterRadius()));
    connect(ui->addTagButton, SIGNAL(clicked(bool)), this, SLOT(addTagShow()));
    connect(ui->nameTagLineEdit, SIGNAL(textChanged(QString)), this, SLOT(enabledTagButton()));
    connect(ui->radius1Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius2Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius3Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));

    emit ui->radiusSlider->valueChanged(50);
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

void SideBar::updateLabelValueRadius(int value)
{
    ui->valueRadiusLabel->setText(QString("%1 %2").arg(value).arg(tr("km")));
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

void SideBar::addTagShow()
{
//    QInputDialog *inputDialog = new QInputDialog(this);
//    inputDialog->setInputMode(QInputDialog::TextInput);
//    inputDialog->setLabelText(tr("Name tag:"));
//    inputDialog->setWindowTitle(tr("Input name tag"));
//    inputDialog->setOkButtonText(tr("Add"));
//    inputDialog->setCancelButtonText(tr("Cancel"));
//    if (inputDialog->exec() == QInputDialog::Accepted) {
//    QLabel *tag = new QLabel(ui->nameTagLineEdit->text());
    QCheckBox *tag = new QCheckBox(ui->nameTagLineEdit->text(), ui->scrollAreaWidgetContents);
//    ui->scrollArea->layout()->addWidget(tag);
    ui->scrollAreaWidgetContents->layout()->addWidget(tag);

//    ui->scrollArea->setGeometry(QRect(QPoint(0, 0), ui->scrollArea->sizeHint()));
    ui->scrollAreaWidgetContents->adjustSize();
}

void SideBar::enabledTagButton()
{
    ui->addTagButton->setEnabled(!ui->nameTagLineEdit->text().isEmpty());
    ui->colorTagToolButton->setEnabled(!ui->nameTagLineEdit->text().isEmpty());
}

void SideBar::showSelectColorTag()
{

}

void SideBar::setRadius()
{
    int value = qobject_cast<QPushButton*>(sender())->text().remove(QRegExp("\\D+")).toInt();
    ui->radiusSlider->setValue(value);
}
