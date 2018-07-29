#include "gpsparser.h"

GPSParser::GPSParser(GPSDataStorage *dataStore, QString location)
{
    this->dataStore = dataStore;
    fileLocation = location;
}

void GPSParser::gpsParse()
{
    dataStore->read(&data);
    for (QString i: data){
        qDebug()<<i;
    }
}

