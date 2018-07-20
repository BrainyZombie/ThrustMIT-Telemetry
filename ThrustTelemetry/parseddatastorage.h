#ifndef PARSEDDATASTORAGE_H
#define PARSEDDATASTORAGE_H

#include <QVector>
#include <QMutex>

class ParsedDataStorage
{
public:
    ParsedDataStorage(QString file);
    bool write(QVector<double>*,QVector<double>*,QVector<double>*,QVector<double>*);
    bool read(QVector<double>*,QVector<double>*,QVector<double>*,QVector<double>*);
    bool write(QVector<double>*,QVector<double>*);
    bool read(QVector<double>*,QVector<double>*);
    QVector<double> T;
    QVector<double> X;
    QVector<double> Y;
    QVector<double> Z;
    QVector<double> W;
    QString fileLocation;
    QMutex mutex;
};

#endif // PARSEDDATASTORAGE_H
