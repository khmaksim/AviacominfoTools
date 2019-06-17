#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
    class MainWidget;
}

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

        void connectDatabase();
};

#endif // MAINWIDGET_H
