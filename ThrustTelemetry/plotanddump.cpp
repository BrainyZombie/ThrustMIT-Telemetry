#include "plotanddump.h"
#include "ui_plotanddump.h"


PlotAndDump::PlotAndDump(int mode, ParsedDataStorage* data) :
    QDialog(0),
  ui(new Ui::PlotAndDump)
{
  ui->setupUi(this);
    this->mode=mode;
    this->data=data;
  lenTarget=200;
  lower=0;
  upper=0;

  X.reserve(20000);
  Y.reserve(20000);
  Z.reserve(20000);
  W.reserve(20000);
  T.reserve(20000);

  graphAreaSetup(data);
  UISetup();
  connect(ui->graphArea, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(updateMousePosition(QMouseEvent*)));
  connect(ui->graphArea, SIGNAL(legendClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendClick2(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)));

  this->setVisible(true);
}


PlotAndDump::~PlotAndDump()
{
    delete ui;
}


void PlotAndDump::legendClick2(QCPLegend* leg,QCPAbstractLegendItem* item,QMouseEvent* mouse)
{
    Q_UNUSED(leg);
    Q_UNUSED(mouse);
    qDebug()<<"yoyo";
  if (item)
  {
      qDebug()<<"yoyo";
      for (int i=0; i<3 && graph[i]; i++){
          QCPPlottableLegendItem *item2 = ui->graphArea->legend->itemWithPlottable(graph[i]);
          if (item == item2){
              graph[i]->setVisible(!graph[i]->visible());
              tag[i]->setVisible(graph[i]->visible());
              mouseTag[i]->setEnabled(graph[i]->visible());
              plotGraph();
              return;
          }
      }
  }
}


void PlotAndDump::graphAreaSetup(ParsedDataStorage* data)
{

    ui->graphArea->setInteraction(QCP::iSelectLegend);
    ui->graphArea->xAxis->setLabel(data->fileLocation.mid(data->fileLocation.length()-8,4));
    ui->graphArea->legend->setVisible(true);
    ui->graphArea->legend->setSelectableParts(QCPLegend::spItems);
      QFont legendFont;
      legendFont.setPointSize(10);
      ui->graphArea->legend->setFont(legendFont);
}

void PlotAndDump::UISetup()
{
    ui->sizeSlider->setRange(50,1000);
    ui->sizeSlider->setSliderPosition(lenTarget);
    ui->sizeText->setText(QString::number(lenTarget));

    ui->topPaddingSlider->setRange(-1000,1000);
    ui->topPaddingSlider->setSliderPosition(upper);
    ui->topPaddingText->setText(QString::number(upper));

    ui->bottomPaddingSlider->setRange(-1000,1000);
    ui->bottomPaddingSlider->setSliderPosition(lower);
    ui->bottomPaddingText->setText(QString::number(lower));

    ui->scroll->setSliderPosition(1);
}

void PlotAndDump::updateMousePosition(QMouseEvent *event){
    mouseX = event->pos().x();
    mouseY = event->pos().y();
    updateMouseTags();
    plotGraph();
}

void PlotAndDump::plot(){
    if (total2==0){
        total2++;
        graphSetup();
    }
    setGraphData();
    updateAxisTags();
    updateUIRanges();
    updateMouseTags();
    plotGraph();
}

void PlotAndDump::addTag(QString tagString, double time)
{
    AxisTag* tag = new AxisTag(graph[0]->valueAxis(), graph[0]->keyAxis(), graph[0]);
    QColor tagColor(Qt::black);
    QPen tagPen(tagColor);
    tag->setPen(tagPen);
    tag->updatePosition(time);
    tag->setText(tagString);
    tag->setVisible(true);
    qDebug()<<tagString<<time;
}



void PlotAndDump::graphSetup()
{
    if (mode==1||mode==3){
        graph[0] = ui->graphArea->addGraph();
        graph[1] = ui->graphArea->addGraph();
        graph[2] = ui->graphArea->addGraph();


        graph[0]->setScatterStyle(QCPScatterStyle::ssNone);
        graph[1]->setScatterStyle(QCPScatterStyle::ssNone);
        graph[2]->setScatterStyle(QCPScatterStyle::ssNone);

        graph[0]->setLineStyle(QCPGraph::lsLine);
        graph[1]->setLineStyle(QCPGraph::lsLine);
        graph[2]->setLineStyle(QCPGraph::lsLine);

        graph[0]->setName("X");
        graph[1]->setName("Y");
        graph[2]->setName("Z");

        ui->graphArea->axisRect()->addAxis(QCPAxis::atRight);
        ui->graphArea->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(200); // add some padding to have space for tags
        ui->graphArea->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(200); // add some padding to have space for tags

        a=new QPen;
        a->setWidth(a->width()*1.5);
        a->setColor((new QColor)->fromRgb(0,0,255));
        graph[0]->setPen(*a);
        a->setColor((new QColor)->fromRgb(255,0,0));
        graph[1]->setPen(*a);
        a->setColor((new QColor)->fromRgb(0,150,0));
        graph[2]->setPen(*a);


        tag[0] = new AxisTag(graph[0]->valueAxis(), graph[0]);
        tag[0]->setPen(graph[0]->pen());
        tag[1] = new AxisTag(graph[1]->valueAxis(), graph[1]);
        tag[1]->setPen(graph[1]->pen());
        tag[2] = new AxisTag(graph[2]->valueAxis(), graph[2]);
        tag[2]->setPen(graph[2]->pen());

        mouseTag[0] = new AxisTag(graph[0]->valueAxis(), graph[0]->keyAxis(), graph[0]);
        mouseTag[0]->setPen(graph[0]->pen());
        mouseTag[1] = new AxisTag(graph[1]->valueAxis(), graph[1]->keyAxis(), graph[1]);
        mouseTag[1]->setPen(graph[1]->pen());
        mouseTag[2] = new AxisTag(graph[2]->valueAxis(), graph[2]->keyAxis(), graph[2]);
        mouseTag[2]->setPen(graph[2]->pen());

    }
    else if (mode==2||mode==4){
        graph[0] = ui->graphArea->addGraph();

        graph[0]->setScatterStyle(QCPScatterStyle::ssNone);

        graph[0]->setLineStyle(QCPGraph::lsLine);

        graph[0]->setName("W");


        ui->graphArea->axisRect()->addAxis(QCPAxis::atRight);
        ui->graphArea->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(100); // add some padding to have space for tags
        ui->graphArea->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(100); // add some padding to have space for tags

        QPen *a=new QPen;
        a->setWidth(a->width()*1.5);
        a->setColor((new QColor)->fromRgb(0,0,255));
        graph[0]->setPen(*a);


        tag[0] = new AxisTag(graph[0]->valueAxis(), graph[0]);
        tag[0]->setPen(graph[0]->pen());

        mouseTag[0] = new AxisTag(graph[0]->valueAxis(), graph[0]->keyAxis(), graph[0]);
        mouseTag[0]->setPen(graph[0]->pen());


    }

    if (mode==3||mode==4){
        ui->clear->setEnabled(false);
        ui->clear->setHidden(true);
    }
}

void PlotAndDump::setGraphData()
{
    if (mode == 1|| mode == 3){
        while(!data->read(&X,&Y,&Z,&T));

        graph[0]->setData(T.mid(position,lenTarget),X.mid(position,lenTarget));
        graph[1]->setData(T.mid(position,lenTarget),Y.mid(position,lenTarget));
        graph[2]->setData(T.mid(position,lenTarget),Z.mid(position,lenTarget));
    }

    else if(mode == 2 || mode == 4){
        while(!data->read(&W,&T));
        graph[0]->setData(T.mid(position,lenTarget),W.mid(position,lenTarget));
    }
}

void PlotAndDump::updateAxisTags()
{
    if (mode == 1|| mode == 3){
        tag[0]->updatePosition(graph[0]->dataMainKey(graph[0]->dataCount()-1));

        tag[1]->updatePosition(graph[1]->dataMainKey(graph[1]->dataCount()-1));

        tag[2]->updatePosition(graph[2]->dataMainKey(graph[2]->dataCount()-1));
    }
     else if(mode == 2 || mode == 4){
        tag[0]->updatePosition(graph[0]->dataMainKey(graph[0]->dataCount()-1));

    }
}

void PlotAndDump::updateUIRanges()
{
    if(mode==1 || mode==2){

        if (ui->scroll->value() == ui->scroll->maximum()){
            ui->scroll->setRange(0,T.length()-lenTarget);
            ui->scroll->setSliderPosition(position = ui->scroll->maximum());
        }
        else{
            ui->scroll->setRange(0,T.length()-lenTarget);
        }
    }

    else if(mode==3 || mode==4){

        ui->scroll->setRange(0,T.length()-lenTarget);
    }
}


void PlotAndDump::updateMouseTags()
{
    double x = ui->graphArea->xAxis->pixelToCoord(mouseX);
    bool a;
    QCPRange range = ui->graphArea->graph(0)->getKeyRange(a);
    if (!a)
        return;

    if (!range.contains(x)){
        mouseTag[0]->setVisible(false);
        if (mode == 1 || mode == 3){
            mouseTag[1]->setVisible(false);
            mouseTag[2]->setVisible(false);
        }
    }
    else
    {
        mouseTag[0]->setVisible(true);
        if (mode == 1 || mode == 3){
            mouseTag[1]->setVisible(true);
            mouseTag[2]->setVisible(true);
        }
    }

    if (mode==1||mode==3){

        mouseTag[0]->updatePosition(x);

        mouseTag[1]->updatePosition(x);

        mouseTag[2]->updatePosition(x);
    }
    else if (mode==2||mode==4){
        mouseTag[0]->updatePosition(x);
    }
}

void PlotAndDump::plotGraph(){
    ui->graphArea->rescaleAxes(true);
    ui->graphArea->yAxis->setRange(ui->graphArea->yAxis->range().lower-lower,ui->graphArea->yAxis->range().upper+upper);
    ui->graphArea->replot();
    ui->graphArea->update();
}

void PlotAndDump::quit(){
    qDebug()<<"close";
    this->close();
}

void PlotAndDump::on_sizeSlider_sliderMoved(int position)
{
    lenTarget=position;
    ui->sizeText->setText(QString::number(position));
    if (mode==3||mode==4){
        ui->scroll->setRange(0,T.length()-lenTarget);
        plot();
    }
}


void PlotAndDump::on_clear_clicked()
{
    X.clear();
    Y.clear();
    Z.clear();
    W.clear();
    T.clear();
}


void PlotAndDump::on_sizeText_editingFinished()
{
    lenTarget=ui->sizeText->text().toInt();
    ui->sizeSlider->setValue(lenTarget);
    if (mode==3||mode==4){
        ui->scroll->setRange(0,T.length()-lenTarget);
        plot();
    }
}

void PlotAndDump::on_topPaddingSlider_sliderMoved(int position)
{
    upper=position;
    ui->topPaddingText->setText(QString::number(upper));
    if (mode==3||mode==4){
        plot();
    }

}

void PlotAndDump::on_bottomPaddingSlider_sliderMoved(int position)
{
    lower=position;
    ui->bottomPaddingText->setText(QString::number(lower));
    if (mode==3||mode==4){
        plot();
    }
}

void PlotAndDump::on_bottomPaddingText_editingFinished()
{
    lower=ui->bottomPaddingText->text().toFloat();
    ui->bottomPaddingSlider->setValue(lower);
    if (mode==3||mode==4){
        plot();
    }
}

void PlotAndDump::on_topPaddingText_editingFinished()
{
    upper=ui->topPaddingText->text().toFloat();
    ui->topPaddingSlider->setValue(upper);
    if (mode==3||mode==4){
        plot();
    }
}

void PlotAndDump::on_scroll_sliderMoved(int position)
{
    this->position=position;
    plot();
}
