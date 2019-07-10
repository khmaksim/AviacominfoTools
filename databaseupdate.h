#ifndef DATABASEUPDATE_H
#define DATABASEUPDATE_H

#include <QObject>
#include <QMap>
#include <QVector>

class DatabaseUpdate : public QObject
{
        Q_OBJECT
    public:
        explicit DatabaseUpdate(QObject *parent = nullptr);
        ~DatabaseUpdate() {}

        void setData(QMap<QString, QString>, QVector<QVector<QString>>);

    public slots:
        void process();
        void stop();

    private:
        QMap<QString, QString> airfield;
        QVector<QVector<QString>> obstracles;

    signals:
        void finished();
};

#endif // DATABASEUPDATE_H
