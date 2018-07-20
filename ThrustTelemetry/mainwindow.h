#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "serialinitial.h"
#include "fileinitial.h"
#include "stringparser.h"
#include <plotanddump.h>
#include "datastorage.h"
#include <serial.h>
#include <QFile.h>
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Serial* port;
    //Creates a new graph that accesses data from provided parsedDataStorage class and sets *dest to pointer to the graph
    void newGraph(int, StringParser*, ParsedDataStorage*, PlotAndDump **dest);
    void otherData(QString);
private slots:
    void on_RealTime_clicked();
    
    void on_File_clicked();

    //initializes serial class with passed arguments, creates and transfers objects to respective threads
    void setSerialConfig(Serial*, QString, int, QString, long long int);

    //for each file passes, reads and sends all data of the file to be parsed and subsequently graphed
    void setFileConfig(QList<QUrl>);


signals:
    void done(int);
    void closed();

private:
    Ui::MainWindow *ui;
    QThread* serialThread;
    QThread* stringParserThread;
    StringParser* parser;
    QFile* file;
    DataStorage* data;


    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
