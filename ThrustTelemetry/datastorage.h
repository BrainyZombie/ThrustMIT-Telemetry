/*CLASS TO STORE AND TRANSFER RAW SERIAL DATA BETWEEN OTHER CLASSES ACROSS THREADS*/

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QMutex>
#include <QObject>

class DataStorage : public QObject
{

public:
    bool serialRawWrite(QString);       //Write unparsed serial data to this class
    bool serialRawRead(QString*);       //Read unparsed serial data from this class
    static QString serialRead;          //static variable to store unparsed data
    static QString fileLocation;        //static variable to store location of files to be read/written
private:
    QMutex m;                           //mutex used for synchronization
};

#endif // DATASTORAGE_H
