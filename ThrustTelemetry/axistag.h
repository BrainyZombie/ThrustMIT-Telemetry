#ifndef axistag
#define axistag
#include <QObject>
#include "qcustomplot.h"

class AxisTag : public QObject
{
    Q_OBJECT
    void config(QCPAxis *mAxis, QCPGraph *graph);
public:
  explicit AxisTag(QCPAxis *parentAxis, QCPGraph *graph);
  explicit AxisTag(QCPAxis *keyAxis, QCPAxis *valueAxis, QCPGraph *graph);
  virtual ~AxisTag();

  // setters:
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setText(const QString &text);

  // getters:
  QPen pen() const { return mLabel->pen(); }
  QBrush brush() const { return mLabel->brush(); }
  QString text() const { return mLabel->text(); }

  // other methods:
  void updatePosition(double value);

  void updatePosition(double valueX, double valueY);
  void setVisible(bool a);
  void setEnabled(bool a);
protected:
  QCPAxis *mAxis;
  QCPAxis *jAxis;
  QPointer<QCPItemTracer> mDummyTracer;
  QPointer<QCPItemLine> mArrow;
  QPointer<QCPItemText> mLabel;
  bool isEnabled;
};
#endif
