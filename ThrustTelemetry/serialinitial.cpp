#include "serialinitial.h"
#include "ui_serialinitial.h"

SerialInitial::SerialInitial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialInitial)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> a = QSerialPortInfo::availablePorts();
    for (int i=0;i<a.length();i++)
        ui->PortList->addItem(a.at(i).portName());
}

SerialInitial::~SerialInitial()
{
    delete ui;
}

void SerialInitial::on_buttonBox_accepted()
{
    QString name=ui->PortList->currentText();
    QString Baud =ui->BaudRate->text();
    QString loopTime = ui->loopAroundTime->text();
    bool check=false;
    QList<QSerialPortInfo> a = QSerialPortInfo::availablePorts();
    for (int i=0;i<a.length();i++)
        if(a.at(i).portName()==name)
            check=true;

    if (name!=NULL&&Baud!=NULL&&fileLocation!=""&&check){
        port= new Serial();
        QDir(fileLocation).mkdir(".");
    }
    else{
        port=NULL;}

    emit serialConfig(port,name,Baud.toInt(),fileLocation, loopTime.toLongLong());
}


void SerialInitial::on_File_clicked()
{
    fileLocationIntermediate=QFileDialog::getExistingDirectory(this,"Path to save data to",QDir::homePath());
    if (fileLocationIntermediate!=""){
        fileLocation=fileLocationIntermediate+"/"+QString("%1").arg(QDateTime::currentSecsSinceEpoch())+"/";
        QMessageBox::information(this,"File location","Files will save in "+fileLocation);
    }
}
