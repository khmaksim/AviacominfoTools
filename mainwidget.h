#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
    class MainWidget;
}

class Database;
class ObstraclesHandler;
class MainWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = 0);
        ~MainWidget();

    public slots:
        void showObstracles();

    private:
        Ui::MainWidget *ui;

        ObstraclesHandler *obstraclesHandler;
        Database *db;
};

#endif // MAINWIDGET_H
