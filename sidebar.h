#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVariant>

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
        bool eventFilter(QObject *watched, QEvent *event);

    private:
        Ui::SideBar *ui;
        QWidget *parent;
        QPropertyAnimation *animationSideBar;

        bool isShown;
        QRect m_geometry;

        float convertCoordInDec(QString);
        void updateTags();
        void updateTypeObstracle();
        void addTagToScrollArea(const QString&);

    private slots:
        void checkBoxChanged(bool);
        void updateLabelValueRadius(int);
        void showAddTag();
        void showSelectColorTag();
        void setRadius();
        void resetFilter();
        void changeArrow();
        void checkBoxTagsChanged();
        void checkBoxTypesChanged();
        void setTagForObstracles(bool);
        void clickedDisplayObstraclesButton();

    signals:
        void searchTextChanged(QString);
        void changedFilterProperty(QString, QVariant);
        void filterRadius();
        void displayObstracles(QVariant);
};

#endif // SIDEBAR_H
