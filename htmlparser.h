#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>

struct Airfield {
    QString codeIcao;
    QString name;
    QString href;
};

class HtmlParser : public QObject
{
        Q_OBJECT
    public:
        enum TypeData { Airfields, Obstracles };

        explicit HtmlParser(TypeData);
        ~HtmlParser() {}

        void setData(QByteArray&);

        QVector<Airfield> getAirfields();

    public slots:
        void process();
        void stop();

    private:
        QByteArray htmlData;
        TypeData type;
        QVector<Airfield> airfields;

    signals:
        void finished();
        void parsedAirfields();
        void parsedObstracles(QVector<QVector<QString>>);
        void parsedPagins(QStringList);
};

#endif // HTMLPARSER_H
