#ifndef GPSPARSER_H
#define GPSPARSER_H

#include <QObject>
#include <QDebug>
#include "gpsdatastorage.h"

class GPSParser : public QObject
{
    Q_OBJECT

public:
    GPSParser(GPSDataStorage*, QString);
    virtual ~GPSParser(){}
public slots:
    void gpsParse();
private:
    GPSDataStorage *dataStore;
    QString fileLocation;
    QVector<QString> data;
};

#endif // GPSPARSER_H
