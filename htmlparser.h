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
        void parseAirfieldsCompleted(QVector<QMap<QString, QString>>);
        void parseObstraclesCompleted(QVector<QVector<QString>>);
};

#endif // HTMLPARSER_H
