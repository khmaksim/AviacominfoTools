#include "settingsobstraclesdialog.h"
#include "ui_settingsobstraclesdialog.h"
#include <QSettings>

SettingsObstraclesDialog::SettingsObstraclesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsObstraclesDialog)
{
    ui->setupUi(this);
    ui->radioButton->setProperty("update_rate", 1);
    ui->radioButton_2->setProperty("update_rate", 7);
    ui->radioButton_3->setProperty("update_rate", 30);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(writeSettings()));

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
    this->accept();
}
