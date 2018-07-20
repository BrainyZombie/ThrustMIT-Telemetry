#include "datastorage.h"

bool DataStorage::serialRawWrite(QString data)
{
    if (m.tryLock()){                           //Lock mutex before writing
        serialRead+=data;                       //append new data
        m.unlock();                             //unlock mutex
        return true;                            //report succesful write
    }
    else return false;                          //report unsuccesful write
}

bool DataStorage::serialRawRead(QString *destination)
{
    if (m.tryLock()){                           //Lock mutex before reading
        *destination=serialRead;                //copy data to destination
        serialRead="";                          //reset class data
        m.unlock();
        return true;                            //report succesful read
    }
    else return false;                          //report unsuccesful read
}


//Initialising static variables
QString DataStorage::serialRead="";
QString DataStorage::fileLocation="";

