#include "sidebar.h"
#include "ui_sidebar.h"
#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QBitmap>
#include "flowlayout.h"
#include "obstraclesform.h"
#include "databaseaccess.h"

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

    updateTags();

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValueRadius(int)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SIGNAL(filterRadius()));
    connect(ui->addTagButton, SIGNAL(clicked(bool)), this, SLOT(showAddTag()));
    connect(ui->radius1Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius2Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->radius3Button, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->resetFilterButton, SIGNAL(clicked(bool)), this, SLOT(resetFilter()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showHide()));
    connect(ui->displayObstraclesButton, SIGNAL(clicked(bool)), this, SLOT(clickedDisplayObstraclesButton()));

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

void SideBar::updateTags()
{
    QVector<QString> tags = DatabaseAccess::getInstance()->getTags();

    if (tags.isEmpty())
        return;

    for (int i = 0; i < tags.size(); i++)
        addTagToScrollArea(tags.at(i));
}

void SideBar::checkBoxChanged(bool f)
{
    emit changedFilterProperty(sender()->objectName(), QVariant(f));
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
    QRegExp regExp("\\D?(\\d{2}|0\\d{2})\\s?(\\d{2})\\s?(\\d{1,2}[\\.\\,]?\\d*)");
    float coordDec = 0;

    if (regExp.indexIn(coordStr) != -1) {
        coordDec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).replace(",", ".").toFloat() / 3600;
    }
    return coordDec;
}

void SideBar::showAddTag()
{
    QInputDialog *inputDialog = new QInputDialog(this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    inputDialog->setInputMode(QInputDialog::TextInput);
    inputDialog->setLabelText(tr("Name tag:"));
    inputDialog->setWindowTitle(tr("Create tag"));
    inputDialog->setOkButtonText(tr("Create"));
    inputDialog->setCancelButtonText(tr("Cancel"));
    if (inputDialog->exec() == QInputDialog::Accepted) {
        if (DatabaseAccess::getInstance()->createTag(inputDialog->textValue())) {
            addTagToScrollArea(inputDialog->textValue());
        }
    }
}

void SideBar::addTagToScrollArea(const QString &nameTag)
{
    QCheckBox *tag = new QCheckBox(nameTag, ui->scrollAreaWidgetContents);
    connect(tag, SIGNAL(clicked(bool)), this, SLOT(setTagForObstracles(bool)));
    connect(tag, SIGNAL(clicked(bool)), this, SLOT(checkBoxTagsChanged()));
    ui->scrollAreaWidgetContents->layout()->addWidget(tag);
    ui->scrollAreaWidgetContents->adjustSize();
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

void SideBar::setTagForObstracles(bool checked)
{
    if (!checked)
        return;
    QVariantList idObstracles = qobject_cast<ObstraclesForm*>(this->parent)->getCheckedObstralcles();
    QCheckBox *selectedTag = qobject_cast<QCheckBox*>(sender());
    DatabaseAccess::getInstance()->setTag(selectedTag->text(), idObstracles);
}

void SideBar::checkBoxTagsChanged()
{
    QStringList tags;
    QList<QCheckBox*> tagsCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox*>();
    for (QList<QCheckBox*>::iterator it = tagsCheckBox.begin(); it != tagsCheckBox.end(); ++it) {
        if ((*it)->isChecked())
            tags << (*it)->text();
    }
    emit changedFilterProperty(sender()->objectName(), QVariant(tags));
}

void SideBar::clickedDisplayObstraclesButton()
{
    emit displayObstracles(QVariant(QPointF(convertCoordInDec(ui->latLineEdit->text()),
                                            convertCoordInDec(ui->lonLineEdit->text()))));
}
