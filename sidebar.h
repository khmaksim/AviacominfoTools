#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
    class SideBar;
}

class SideBar : public QWidget
{
        Q_OBJECT

    public:
        explicit SideBar(QWidget *parent = 0);
        ~SideBar();

    protected:
        QSize sizeHint();

    private:
        Ui::SideBar *ui;

    private slots:
        void checkBoxChanged(bool);

    signals:
        void searchTextChanged(QString);
        void toggled(QString, bool);
};

#endif // SIDEBAR_H
