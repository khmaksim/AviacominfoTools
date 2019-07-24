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
        void updateTypeObstracleFilter(const QStringList);

    public slots:
        void showHide();
        void resetFilter();

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

        void writeSettings();
        void readSettings();
        float convertCoordInDec(QString);
        void updateTags();
        void addTagToScrollArea(const QString&);

    private slots:
        void checkBoxChanged(bool);
        void updateLabelValueRadius(int);
        void showAddTag();
        void showSelectColorTag();
        void setRadius();
        void changeArrow();
        void checkBoxTagsChanged();
        void checkBoxTypesChanged();
        void setTagForObstracles(bool);
        void clickedDisplayObstraclesButton();
        void heightFilterChanged();
        void enabledRemoveTagButton();
        void removeTag();

    signals:
        void searchTextChanged(QString);
        void changedFilterProperty(QString, QVariant);
        void filterRadius();
        void displayObstracles(QVariant);
};

#endif // SIDEBAR_H
