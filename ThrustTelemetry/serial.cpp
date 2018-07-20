#include "serial.h"
#include "mainwindow.h"
#include <QDebug.h>

int total=0;

Serial::Serial(){}

void Serial::start(QString portName, int baudRate){
    port = new QSerialPort();
    port->setPortName(portName);
    port->setBaudRate(baudRate);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->open(QIODevice::ReadOnly);

    connect(port,SIGNAL(readyRead()),this,SLOT(serialRecieved()));
}
void Serial::serialRecieved()
{   serialRead.append(port->readAll());
    total++;
    //discarding first 50 data transmissions
    if (total<50){ serialRead=""; return;}
    if (data.serialRawWrite(serialRead)){
        serialRead="";
        emit(written(0));
    }
}
