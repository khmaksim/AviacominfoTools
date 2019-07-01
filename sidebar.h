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

        float getLat();
        float getLon();
        int getRadius();

    protected:
        QSize sizeHint();

    private:
        Ui::SideBar *ui;

        float convertCoordInDec(QString);

    private slots:
        void checkBoxChanged(bool);
        void updateLabelValueRadius(int);
        void addTagShow();
        void enabledTagButton();
        void showSelectColorTag();
        void setRadius();

    signals:
        void searchTextChanged(QString);
        void toggled(QString, bool);
        void filterRadius();
};

#endif // SIDEBAR_H
