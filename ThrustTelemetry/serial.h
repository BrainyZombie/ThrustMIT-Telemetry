#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include "datastorage.h"

class Serial : public QObject
{

    Q_OBJECT

public:
    Serial();
    void start(QString PortName, int BaudRate);
private:
    QSerialPort* port;
    QString serialRead;
    DataStorage data;


public slots:
    void serialRecieved();

signals:
    void written(int);
};

#endif // SERIAL_H
