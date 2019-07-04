#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
    class SideBar;
}

class QPropertyAnimation;
class SideBar : public QWidget
{
        Q_OBJECT

    public:
        explicit SideBar(QWidget *parent = 0);
        ~SideBar();

        float getLat();
        float getLon();
        int getRadius();

    public slots:
        void showHide();

    protected:
        QSize sizeHint();
        void resizeEvent(QResizeEvent *event);
//        void mouseMoveEvent(QMouseEvent *event);
        void leaveEvent(QEvent*);
        void enterEvent(QEvent*);

    private:
        Ui::SideBar *ui;
        QTimer *timer;

        QWidget *parent;
        QPropertyAnimation *animationSideBar;

        bool isShown;
        QRect m_geometry;

        float convertCoordInDec(QString);

    private slots:
        void checkBoxChanged(bool);
        void updateLabelValueRadius(int);
        void addTagShow();
        void showSelectColorTag();
        void setRadius();
        void resetFilter();
        void changeArrow();

    signals:
        void searchTextChanged(QString);
        void toggled(QString, bool);
        void filterRadius();
};

#endif // SIDEBAR_H
