#include "gpsdatastorage.h"

GPSDataStorage::GPSDataStorage()
{

}

bool GPSDataStorage::write(QString dataInput)
{
    if (m.tryLock()){
        gpsData.push_back(dataInput);
        m.unlock();
        return true;
    }
    else return false;
}

bool GPSDataStorage::read(QVector<QString> *dataOutput)
{
    if (m.tryLock()){
        dataOutput->append(gpsData);
        gpsData.clear();
        m.unlock();
        return true;
    }
    else return false;
}
