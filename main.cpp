#include "mainwidget.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Aviacominfo");
    QCoreApplication::setOrganizationDomain("aviacominfo.com");
    QCoreApplication::setApplicationName("AviacominfoTools");

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("aviacominfotools"), QLatin1String("_"), QLatin1String(":/translations/res/translations")))
        a.installTranslator(&translator);

    MainWidget w;
    w.show();

    return a.exec();
}
