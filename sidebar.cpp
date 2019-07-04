#include "sidebar.h"
#include "ui_sidebar.h"
#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QBitmap>
#include "flowlayout.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, QBrush(QColor(128, 128, 128)));
//    setPalette(palette);
//    QPixmap maskPix(":/images/res/img/mask.png");
//    setMask(maskPix.scaled(size()).mask());

    isShown = false;
    this->parent = parent;
    timer = new QTimer(this);

    ui->minRadiusLabel->setNum(ui->radiusSlider->minimum());
    ui->maxRadiusLabel->setNum(ui->radiusSlider->maximum());

    ui->scrollAreaWidgetContents->setLayout(new FlowLayout());

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValueRadius(int)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SIGNAL(filterRadius()));
    connect(ui->addTagButton, SIGNAL(clicked(bool)), this, SLOT(addTagShow()));
    connect(ui->radius1Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius2Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius3Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->resetFilterButton, SIGNAL(clicked(bool)), this, SLOT(resetFilter()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showHide()));

    emit ui->radiusSlider->valueChanged(50);
}

SideBar::~SideBar()
{
    delete ui;
}

void SideBar::enterEvent(QEvent *)
{
    if (!isShown)
        showHide();
    if (timer->isActive())
        timer->stop();
}

void SideBar::leaveEvent(QEvent *)
{
    timer->start(5000);
}

void SideBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (isShown)
        showHide();
    QPixmap maskPix(":/images/res/img/mask.png");
    ui->label->setMask(maskPix.scaled(ui->label->size()).mask());
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
    QInputDialog *inputDialog = new QInputDialog(this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    inputDialog->setInputMode(QInputDialog::TextInput);
    inputDialog->setLabelText(tr("Name tag:"));
    inputDialog->setWindowTitle(tr("Input name tag"));
    inputDialog->setOkButtonText(tr("Add"));
    inputDialog->setCancelButtonText(tr("Cancel"));
    if (inputDialog->exec() == QInputDialog::Accepted) {
        QCheckBox *tag = new QCheckBox(inputDialog->textValue(), ui->scrollAreaWidgetContents);
        //    ui->scrollArea->layout()->addWidget(tag);
        ui->scrollAreaWidgetContents->layout()->addWidget(tag);

        //    ui->scrollArea->setGeometry(QRect(QPoint(0, 0), ui->scrollArea->sizeHint()));
        ui->scrollAreaWidgetContents->adjustSize();
    }
}

void SideBar::showSelectColorTag()
{

}

void SideBar::setRadius()
{
    int value = qobject_cast<QPushButton*>(sender())->text().remove(QRegExp("\\D+")).toInt();
    ui->radiusSlider->setValue(value);
}

void SideBar::resetFilter()
{
    ui->markingDayCheckBox->setChecked(false);
    ui->nightMarkingCheckBox->setChecked(false);
    ui->radiusGroupBox->setChecked(false);
    ui->latLineEdit->clear();
    ui->lonLineEdit->clear();
    ui->radiusSlider->setValue(50);
}

void SideBar::showHide()
{
    animationSideBar = new QPropertyAnimation(this, "geometry");
    animationSideBar->setDuration(300);

    QRect startRect(parent->width() - 20, y(), width(), height());
    QRect endRect(parent->width() - width(), y(), width(), height());

    if (!isShown) {
        animationSideBar->setStartValue(startRect);
        animationSideBar->setEndValue(endRect);
    }
    else {
        animationSideBar->setStartValue(endRect);
        animationSideBar->setEndValue(startRect);
    }

    animationSideBar->start();
    QTimer::singleShot(animationSideBar->duration(), this, SLOT(changeArrow()));

    isShown = !isShown;
    if (timer->isActive())
        timer->stop();
}

void SideBar::changeArrow()
{
    if (isShown)
        ui->label->setPixmap(QPixmap(":/images/res/img/arrow-right.png"));
    else
        ui->label->setPixmap(QPixmap(":/images/res/img/arrow-left.png"));
}
