#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port=NULL;
    data = new DataStorage;


    QObject *parent2;
        QString program = "C:/Program Files/Google/Google Earth Pro/client/googleearth.exe";
        QStringList arguments;

        QProcess *myProcess = new QProcess(parent2);
        myProcess->start(program, arguments);
}

void MainWindow::on_RealTime_clicked()
{
    //display a window to collect data for serial plotting
    SerialInitial* a= new SerialInitial(this);
    connect(a,SIGNAL(serialConfig(Serial*,QString,int,QString, long long int)),this,SLOT(setSerialConfig(Serial*,QString,int,QString, long long int)));
    a->setModal(true);
    a->show();
}

void MainWindow::setSerialConfig(Serial* port,QString portName, int baudRate, QString fileLocation, long long int loopTime){

    this->port=port;

    if (port==NULL)
        QMessageBox::about(this, "Error","Invalid Serial Config");

    else{
        DataStorage::fileLocation=fileLocation;

        //creating string parser
        parser = new StringParser(loopTime);


        //connecting signals and slots
        connect(port,SIGNAL(written(int)),parser,SLOT(read(int)));  //Signal-slot to fascilitate serial telling the stringparse to read
        connect(parser,StringParser::newGraph,this,MainWindow::newGraph);   //Signal-slot to let stringParser to instruct mainwindow to create a new graph
        connect(parser,StringParser::otherData,this,MainWindow::otherData);

        port->start(portName, baudRate);
    }
}

void MainWindow::on_File_clicked(){
    //create new dialog box for setting file parameters
    FileInitial* a= new FileInitial(this);

    //connect it to corresponding slot in mainWindow
    connect(a,SIGNAL(fileConfig(QList<QUrl>)),this,SLOT(setFileConfig(QList<QUrl>)));
    a->setModal(true);
    a->show();
}


//slot to recieve file config data from another window
void MainWindow::setFileConfig(QList<QUrl> files){

    //process each file one by one
    for (auto e: files){
        stringParserThread = new QThread;
        file = new QFile(e.toLocalFile());
        file->open(QIODevice::ReadWrite);

        //check for value after which time field loops back to zero, I.E. max value of time
        QString checkLoopTime = file->readLine();

        if (checkLoopTime.contains("Time:")){
            parser = new StringParser(checkLoopTime.mid(5).toLongLong());
        }
        else{
            parser = new StringParser(0);
        }

        //create a string parser and move it to a new thread to improve efficiency
        parser->moveToThread(stringParserThread);

        //connect slot to allow the stringparser to request the mainwindow to create a new graph
        connect(parser,StringParser::newGraph,this,MainWindow::newGraph);

        //connect slot to allow the stringparser to request the mainwindow to display unplottable data as string
        connect(parser,StringParser::otherData,this,MainWindow::otherData);

        //connect slot to allow passing of raw data from mainwindow to the stringparser using a Mutexed class, int represents mode, file(2) or serial(0)
        connect(this,SIGNAL(done(int)),parser,SLOT(read(int)));

        stringParserThread->start();
        file->open(QIODevice::ReadOnly);

        //read the whole file and send it to the parser
        while(!data->serialRawWrite(file->readAll()));
        emit done(2);
        data = new DataStorage;
    }
}


void MainWindow::newGraph(int mode, StringParser* parser, ParsedDataStorage* data, PlotAndDump** dest){
    PlotAndDump* plot = new PlotAndDump(mode,data);
    connect(this, SIGNAL(closed()), plot, SLOT(quit()));
    connect(parser, SIGNAL(addTag(QString, double)), plot, SLOT(addTag(QString, double)));
    connect(parser,SIGNAL(plot()), plot, SLOT(plot()));
    *dest=plot;
}

void MainWindow::closeEvent(QCloseEvent *event){
    emit closed();
    event->accept();
}

void MainWindow::otherData(QString data){
    if (!ui->widget_2->isEnabled()){
        ui->widget_2->setEnabled(true);
        ui->unplottedData->setEnabled(true);
        ui->widget_2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        ui->unplottedData->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    ui->unplottedData->setPlainText(ui->unplottedData->toPlainText()+data);
}
