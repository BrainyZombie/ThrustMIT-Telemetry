 #include "stringparser.h"

StringParser::StringParser(long long int time, QString source)
{
    serialData="";
    serialData2="";
    file=NULL;
    stream=NULL;
    ok=true;
    loopTime = time;
    loopNo = 0;
    this->source = source;
    gpsParser = nullptr;
    gpsData = nullptr;
}

void StringParser::writeRaw(){
    if (file==NULL){
        file= new QFile(data.fileLocation+"RAW");
        file->open(QIODevice::ReadWrite | QIODevice::Append);
    }
    if(stream==NULL){
        stream=new QTextStream(file);
        (*stream)<<"Time:"<<QString::number(loopTime)<<"\r\n";
    }
    (*stream)<<serialData2;
    stream->flush();
    file->flush();
}

void StringParser::read(int mode){
    while(!data.serialRawRead(&serialData2));
    serialData+=serialData2;
    if (mode==0){writeRaw();}

    serialData2="";
    do{
        flag=0;
        sensor=serialData.indexOf(':');
        X=serialData.indexOf('X');
        Y=serialData.indexOf('Y');
        Z=serialData.indexOf('Z');
        T=serialData.indexOf('T');
        W=serialData.indexOf('W');
        nextLine=serialData.indexOf('\n');
        GPS=serialData.left(nextLine+1).indexOf("$GP");


        if (sensor==4&&X==5&&Y>X&&Z>Y&&T>Z&&nextLine>T){
            flag=1;
        }

        else if (sensor==4&&W==5&&T>W&&nextLine>T)
            flag=2;

        else if(nextLine==-1)
            flag=-1;

        else
            flag=0;

        if (flag!=-1){
            if (flag!=0){
                currentStream=serialData.left(sensor);
                if (!map.contains(currentStream)){
                    map[currentStream]=new dataPoint;
                    current = map[currentStream];
                    current->mode=flag;
                    current->data=new ParsedDataStorage(source+":"+currentStream);
                    current->graph=NULL;
                    emit(newGraph(flag+mode, this, current->data, &current->graph));
                    int l;
                    while(current->graph==NULL){
                        l++;}
                }
                current=map[currentStream];
            }
            if (flag==1){
                X2=serialData.mid(X+1,Y-X-1).toDouble(&ok);
                if (!ok) {goto a;}
                Y2=serialData.mid(Y+1,Z-Y-1).toDouble(&ok);
                if (!ok) {goto a;}
                Z2=serialData.mid(Z+1,T-Z-1).toDouble(&ok);
                if (!ok) {goto a;}
                T2=serialData.mid(T+1,nextLine-T-1).toDouble(&ok);
                if (!ok) {
                    goto a;}
                if (T2<lastTime){
                    loopNo++;
                }
                lastTime = T2;
                current->X.append(X2);
                current->Y.append(Y2);
                current->Z.append(Z2);
                current->T.append(T2+loopTime*loopNo);
            }
            else if (flag==2){
                W2=serialData.mid(W+1,T-W-1).toDouble(&ok);
                if (!ok) {
                    goto a;}
                T2=serialData.mid(T+1,nextLine-T-1).toDouble(&ok);
                if (!ok) {
                    goto a;}
                if (T2<lastTime){
                    loopNo++;
                }
                lastTime = T2;
                current->W.append(W2);
                current->T.append(T2+loopTime*loopNo);
            }
            else if (flag==0){
            a:  QString midData =serialData.mid(0,nextLine+1);
                emit otherData(midData);

                //handling of adding tag
                delim1=midData.indexOf('[');
                delim2=midData.indexOf(']');
                if (delim1!= -1 && delim2 != -1) {
                    writeAndPlot();
                    emit addTag(serialData.mid(delim1+1, delim2-1), (double)lastTime+loopTime*loopNo);
                    if (mode == 1){
                        QThread::msleep(10);
                    }
                }


                //handling of GPS data
                qDebug()<<GPS<<nextLine;
                if (GPS<nextLine && GPS!=-1){
                    if (gpsParser==nullptr){
                        gpsData = new GPSDataStorage();
                        gpsParser = new GPSParser(gpsData, data.fileLocation);
                        gpsThread = new QThread();
                        gpsParser->moveToThread(gpsThread);
                        connect(this, SIGNAL(gpsWritten()), gpsParser, SLOT(gpsParse()));
                        gpsThread->start();
                    }
                    while (!gpsData->write(serialData.mid(GPS,nextLine-GPS)))
                        QThread::usleep(1);

                    emit(gpsWritten());
                }
            }
            serialData.remove(0,nextLine+1);
        }
    }while(flag!=-1);
    writeAndPlot();
}

void StringParser::writeAndPlot(){
    for (auto e : map.keys()){
        current=map[e];
        if (current->mode==1){
            while(!current->data->write(&current->X,&current->Y,&current->Z,&current->T));
        }
        else if (current->mode==2){
            while(!current->data->write(&current->W,&current->T));
        }
    }
    emit plot();
}
