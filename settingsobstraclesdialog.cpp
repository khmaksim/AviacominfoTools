#include "settingsobstraclesdialog.h"
#include "ui_settingsobstraclesdialog.h"
#include <QSettings>
#include <QtDebug>

SettingsObstraclesDialog::SettingsObstraclesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsObstraclesDialog)
{
    ui->setupUi(this);
    ui->radioButton->setProperty("update_rate", 1);
    ui->radioButton_2->setProperty("update_rate", 7);
    ui->radioButton_3->setProperty("update_rate", 30);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(writeSettings()));
    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));

    readSettings();
}

SettingsObstraclesDialog::~SettingsObstraclesDialog()
{
    delete ui;
}

void SettingsObstraclesDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("database");
    int updateRate = settings.value("update_rate", 1).toInt();
    switch (updateRate) {
        case 1:
            ui->radioButton->setChecked(true);
            break;
        case 7:
            ui->radioButton_2->setChecked(true);
            break;
        case 30:
            ui->radioButton_3->setChecked(true);
            break;
        default:
            ui->radioButton->setChecked(true);
    }
    settings.endGroup();
    if (settings.value("unit").toString().contains("metre"))
        ui->metreRadioButton->setChecked(true);
    else
        ui->footRadioButton->setChecked(true);
}

void SettingsObstraclesDialog::writeSettings()
{
    QSettings settings;

    for (auto it = ui->groupBox->children().begin(); it != ui->groupBox->children().end(); ++it) {
        QRadioButton *button = qobject_cast<QRadioButton*>(*it);
        if (button && button->isChecked()) {
            settings.beginGroup("database");
            settings.setValue("update_rate", button->property("update_rate"));
            settings.endGroup();
            break;
        }
    }
    settings.setValue("unit", ui->metreRadioButton->isChecked() ? "metre" : "foot");

    this->accept();
}
