#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>
#include <QMap>

class HtmlParser : public QObject
{
        Q_OBJECT
    public:
        enum TypeData { Airfields, Obstracles };

        explicit HtmlParser(TypeData);
        ~HtmlParser() {}

        void setData(QByteArray&);

    public slots:
        void process();
        void stop();

    private:
        QByteArray htmlData;
        TypeData type;

    signals:
        void finished();
        void parsedAirfields(QVector<QMap<QString, QString>>);
        void parsedObstracles(QVector<QVector<QString>>);
        void parsedPagins(QStringList);
};

#endif // HTMLPARSER_H
