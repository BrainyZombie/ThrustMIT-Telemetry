#ifndef SERIALINITIAL_H
#define SERIALINITIAL_H

#include <QDialog>
#include "serial.h"
#include "mainwindow.h"
#include "QSerialPortInfo"
#include "QFileDialog"
#include <QMessageBox>
#include <QDateTime>

namespace Ui {
class SerialInitial;
}

class SerialInitial : public QDialog
{
    Q_OBJECT

public:
    explicit SerialInitial( QWidget *parent = NULL);
    ~SerialInitial();

private slots:
    void on_buttonBox_accepted();

    void on_File_clicked();

signals:
    void serialConfig(QString,QString,QString,QString);

private:
    Ui::SerialInitial *ui;
    QFileDialog* file;
    QString fileLocation;
};

#endif // SERIALINITIAL_H
