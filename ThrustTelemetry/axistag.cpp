#include "axisTag.h"

AxisTag::AxisTag(QCPAxis *parentAxis, QCPGraph* graph) :
  QObject(parentAxis),
  mAxis(parentAxis)
{
  mDummyTracer = new QCPItemTracer(mAxis->parentPlot());
  mDummyTracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
  mDummyTracer->position->setAxes(0, mAxis);
  config(mAxis, graph);
  setEnabled(true);
}





AxisTag::AxisTag(QCPAxis *valueAxis, QCPAxis *keyAxis, QCPGraph *graph) :
  QObject(valueAxis),
  mAxis(valueAxis),
  jAxis(keyAxis)
{
    mDummyTracer = new QCPItemTracer(mAxis->parentPlot());
    mDummyTracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
    mDummyTracer->position->setAxes(jAxis, mAxis);
    config(mAxis, graph);
    setEnabled(true);
}

void AxisTag::config(QCPAxis *mAxis, QCPGraph *graph){

    mDummyTracer->setVisible(true);
    mDummyTracer->setStyle(QCPItemTracer::tsNone);
    mDummyTracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
    mDummyTracer->position->setAxisRect(mAxis->axisRect());
    mDummyTracer->position->setCoords(0, 0);
    mDummyTracer->setGraph(graph);

    mArrow = new QCPItemLine(mAxis->parentPlot());
    mArrow->setLayer("overlay");
    mArrow->setClipToAxisRect(false);
    mArrow->setHead(QCPLineEnding::esSpikeArrow);
    mArrow->end->setParentAnchor(mDummyTracer->position);
    mArrow->start->setParentAnchor(mArrow->end);
    mArrow->start->setCoords(15, 0);

    mLabel = new QCPItemText(mAxis->parentPlot());
    mLabel->setLayer("overlay");
    mLabel->setClipToAxisRect(false);
    mLabel->setPadding(QMargins(3, 0, 3, 0));
    mLabel->setBrush(QBrush(Qt::white));
    mLabel->setPen(QPen(Qt::blue));
    mLabel->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mLabel->position->setParentAnchor(mArrow->start);
}


AxisTag::~AxisTag()
{
  if (mDummyTracer)
    mDummyTracer->parentPlot()->removeItem(mDummyTracer);
  if (mArrow)
    mArrow->parentPlot()->removeItem(mArrow);
  if (mLabel)
    mLabel->parentPlot()->removeItem(mLabel);
}

void AxisTag::setPen(const QPen &pen)
{
  mArrow->setPen(pen);
  mLabel->setPen(pen);
}

void AxisTag::setBrush(const QBrush &brush)
{
  mLabel->setBrush(brush);
}

void AxisTag::setText(const QString &text)
{
  mLabel->setText(text);
}

void AxisTag::updatePosition(double valueX)
{
  mDummyTracer->setGraphKey(valueX);
  double valueY = mDummyTracer->position->value();
  mDummyTracer->position->setCoords(valueX, valueY);
  setText(QString::number(valueY, 'f', 2));
}

void AxisTag::updatePosition(double valueX, double valueY)
{
  mDummyTracer->setStyle(QCPItemTracer::tsCrosshair);
  mDummyTracer->position->setCoords(valueX, valueY);
  mDummyTracer->setVisible(true);
}


void AxisTag::setVisible(bool a){
    mArrow->setVisible(a&&isEnabled);
    mLabel->setVisible(a&&isEnabled);
}

void AxisTag::setEnabled(bool a)
{
    isEnabled = a;
}
