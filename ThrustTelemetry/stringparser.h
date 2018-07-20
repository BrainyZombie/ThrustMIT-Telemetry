#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include <QObject>
#include <parseddatastorage.h>
#include <plotanddump.h>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <datastorage.h>

class StringParser : public QObject
{
    Q_OBJECT

public:
    QString serialData2;
    StringParser(long long time);
private:
    QString serialData;
    QString currentStream;
    DataStorage data;
    QFile* file;
    QTextStream* stream;
    int sensor, X, Y, Z, T, W, nextLine, delim1, delim2;
    double X2, Y2, Z2, T2, W2;
    struct dataPoint{
        ParsedDataStorage* data;
        PlotAndDump* graph;
        QVector<double> X;
        QVector<double> Y;
        QVector<double> Z;
        QVector<double> W;
        QVector<double> T;
        int mode;
    } *current;
    QMap<QString,dataPoint*> map;
    int flag;
    bool ok;
    long long int loopTime = 0;
    long long int lastTime = 0;
    int loopNo = 0;
    void writeRaw();
    void writeAndPlot();
signals:
    void plot();
    void otherData(QString);
    PlotAndDump* newGraph(int, StringParser*, ParsedDataStorage*, PlotAndDump**);
    void addTag(QString, double);

public slots:
    void read(int mode = 0);
};

#endif // STRINGPARSER_H
