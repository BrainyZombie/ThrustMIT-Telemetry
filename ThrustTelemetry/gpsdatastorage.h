#ifndef GPSDATASTORAGE_H
#define GPSDATASTORAGE_H

#include<QString>
#include<QVector>
#include<QMutex>

class GPSDataStorage
{
public:
    GPSDataStorage();
    bool write(QString);
    bool read(QVector<QString>*);

private:
    QVector<QString> gpsData;
    QMutex m;
};

#endif // GPSDATASTORAGE_H
