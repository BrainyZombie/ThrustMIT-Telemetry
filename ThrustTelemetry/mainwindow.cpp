#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Q_INIT_RESOURCE(resources);
    QPixmap Logo(":/logo.png");
    QPixmap Logo2(":/logo2.png");
    ui->logo->setPixmap(Logo);
    ui->logo2->setPixmap(Logo2);
    port=NULL;
    data = new DataStorage;
    resize(QGuiApplication::primaryScreen()->availableSize()*4/5);
}

void MainWindow::on_File_clicked(){
    QList<QUrl> files =QFileDialog::getOpenFileUrls(nullptr,QString(),QUrl(),"*");                                       //open dialog to select multiple files with ".txt" extension
    setFileConfig(files);     //pass the files selected
}


//slot to recieve file config data from another window
void MainWindow::setFileConfig(QList<QUrl> files){

    //process each file one by one
    for (QUrl e: files){
        stringParserThread = new QThread;
        file = new QFile(e.toLocalFile());
        file->open(QIODevice::ReadWrite);

        //check for value after which time field loops back to zero, I.E. max value of time
        QString checkLoopTime = file->readLine();

        if (checkLoopTime.contains("Time:")){
            parser = new StringParser(checkLoopTime.mid(5).toLongLong(),e.fileName());
        }
        else{
            parser = new StringParser(0, e.fileName());
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

        //read the whole file and send it to the parser
        while(!data->serialRawWrite(file->readAll()));
        emit done(2);
        data = new DataStorage;
        file->close();
    }
}


void MainWindow::newGraph(int mode, StringParser* parser, ParsedDataStorage* data, PlotAndDump** dest){
    PlotAndDump* plot = new PlotAndDump(mode,data);
    ui->tabWidget->addTab(plot,data->source);
    connect(this, SIGNAL(closed()), plot, SLOT(quit()));
    connect(plot,SIGNAL(toggleWindowMode(PlotAndDump*)), this, SLOT(toggleWindowModes(PlotAndDump*)));
    connect(parser, SIGNAL(addTag(QString, double)), plot, SLOT(addTag(QString, double)));
    connect(parser,SIGNAL(plot()), plot, SLOT(plot()));
    *dest=plot;
}

void MainWindow::closeEvent(QCloseEvent *event){
    emit closed();
    event->accept();
}

void MainWindow::otherData(QString data){

    if (unplottedData == nullptr){
        unplottedData = new QPlainTextEdit();
        ui->tabWidget->addTab(unplottedData, "Unparsable data");
    }
    unplottedData->appendPlainText(data);
}

void MainWindow::launchMap()
{
    QObject *parent2 = new QObject();
    QString program = "C:/Program Files/Google/Google Earth Pro/client/googleearth.exe";
    QStringList arguments;

    QProcess *myProcess = new QProcess(parent2);
    myProcess->start(program, arguments);
}

void MainWindow::toggleWindowModes(PlotAndDump *src)
{
    qDebug()<<1;
    disconnect(this, SIGNAL(closed()), src, SLOT(quit()));
    disconnect(src,SIGNAL(toggleWindowMode(PlotAndDump*)), this, SLOT(toggleWindowModes(PlotAndDump*)));
    disconnect(parser, SIGNAL(addTag(QString, double)), src, SLOT(addTag(QString, double)));
    disconnect(parser,SIGNAL(plot()), src, SLOT(plot()));
    PlotAndDump* plot2 = new PlotAndDump(src);

    connect(this, SIGNAL(closed()), plot2, SLOT(quit()));
    connect(plot2,SIGNAL(toggleWindowMode(PlotAndDump*)), this, SLOT(toggleWindowModes(PlotAndDump*)));
    connect(parser, SIGNAL(addTag(QString, double)), plot2, SLOT(addTag(QString, double)));
    connect(parser,SIGNAL(plot()), plot2, SLOT(plot()));

    if (src->currentWindowMode == src->windowMode::in){
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(src));
        plot2->show();
    }
    else{
        ui->tabWidget->addTab(plot2, plot2->data->source);
    }


}




void MainWindow::on_configSerial_clicked()
{
    //display a window to collect data for serial plotting
    SerialInitial* a= new SerialInitial(this);
    connect(a,SIGNAL(serialConfig(QString,int,QString, long long int)),this,SLOT(setSerialConfig(QString,int,QString, long long int)));
    a->setModal(true);
    a->show();
}


void MainWindow::setSerialConfig(QString portName, int baudRate, QString fileLocation, long long int loopTime){
    Q_INIT_RESOURCE(resources);
    QFile serialConfig("serialConfig.txt");
    serialConfig.open(QIODevice::WriteOnly);
    serialConfig.write(portName.toUtf8());
    serialConfig.write("\n");
    serialConfig.write((new QString(baudRate))->toUtf8());
    serialConfig.write("\n");
    serialConfig.write(fileLocation.toUtf8());
    serialConfig.write("\n");
    serialConfig.write((new QString((int)loopTime))->toUtf8());
    serialConfig.write("\n");
    serialConfig.flush();
    serialConfig.close();
}

void MainWindow::on_startSerial_clicked()
{
    port= new Serial();
    QFile serialConfig("serialConfig.txt");
    QString portName, fileLocation;
    int baudRate, loopTime;
    serialConfig.open(QFile::ReadOnly);
    portName = serialConfig.readLine().trimmed();
    baudRate = serialConfig.readLine().toInt();
    fileLocation = serialConfig.readLine().trimmed();
    loopTime = serialConfig.readLine().toInt();

    DataStorage::fileLocation=fileLocation+"/" + QDateTime::currentMSecsSinceEpoch()+"/";
    stringParserThread = new QThread;
    parser = new StringParser(loopTime, portName);
    parser->moveToThread(stringParserThread);
    stringParserThread->start();


    //connecting signals and slots
    connect(port,SIGNAL(written(int)),parser,SLOT(read(int)));  //Signal-slot to fascilitate serial telling the stringparse to read
    connect(parser,StringParser::newGraph,this,MainWindow::newGraph);   //Signal-slot to let stringParser to instruct mainwindow to create a new graph
    connect(parser,StringParser::otherData,this,MainWindow::otherData);

    port->start(portName, baudRate);
}
