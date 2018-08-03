#ifndef GPSPARSER_H
#define GPSPARSER_H

#include <QObject>
#include <QDebug>
#include<QFile>
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
    QFile kml;
    QString kmlPre, kmlMain, kmlPost;
    bool parseNMEA(char*);
    void editKML();
    void launchMap();
    uint8_t parseHex(char c);

    uint8_t hour, minute, seconds, year, month, day;
     uint16_t milliseconds;
     // Floating point latitude and longitude value in degrees.
     float latitude, longitude;
     // Fixed point latitude and longitude value with degrees stored in units of 1/100000 degrees,
     // and minutes stored in units of 1/100000 degrees.  See pull #13 for more details:
     //   https://github.com/adafruit/Adafruit-GPS-Library/pull/13
     int32_t latitude_fixed, longitude_fixed;
     float latitudeDegrees, longitudeDegrees;
     float geoidheight, altitude;
     float speed, angle, magvariation, HDOP;
     char lat, lon, mag;
     bool fix;
     uint8_t fixquality, satellites;
    double latitude_double, longitude_double;
     bool firstPoint;

     void updateKml();
     void initKml();
};

#endif // GPSPARSER_H
