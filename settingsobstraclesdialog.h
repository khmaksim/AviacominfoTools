#ifndef SETTINGSOBSTRACLESDIALOG_H
#define SETTINGSOBSTRACLESDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsObstraclesDialog;
}

class SettingsObstraclesDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit SettingsObstraclesDialog(QWidget *parent = nullptr);
        ~SettingsObstraclesDialog();

    private:
        Ui::SettingsObstraclesDialog *ui;

        void readSettings();

    private slots:
        void writeSettings();
};

#endif // SETTINGSOBSTRACLESDIALOG_H
