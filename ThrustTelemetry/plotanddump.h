#ifndef PLOTANDDUMP_H
#define PLOTANDDUMP_H

#include <QDialog>
#include "parseddatastorage.h"
#include "qcustomplot.h"
#include "axistag.h"

namespace Ui {
class PlotAndDump;
}

class PlotAndDump : public QDialog
{
    Q_OBJECT

public:
    PlotAndDump(int mode, ParsedDataStorage* data);
    ~PlotAndDump();
    int mode;
    ParsedDataStorage* data;
    QVector<double> X;
    QVector<double> Y;
    QVector<double> Z;
    QVector<double> W;
    QVector<double> T;
    int total2=0;
    int lenTarget;
    QPen *a;
    QCPRange range;
    float lower;
    float upper;
    int position;

    enum windowMode{
        in,
        out
    } currentWindowMode;
    
    PlotAndDump(PlotAndDump *src);

signals:
    void toggleWindowMode(PlotAndDump*);

public slots:
    void plot();
    void addTag(QString, double);
    void quit();

private slots:
    void on_sizeSlider_sliderMoved(int position);

    void on_clear_clicked();

    void on_sizeText_editingFinished();

    void on_topPaddingSlider_sliderMoved(int position);

    void on_bottomPaddingSlider_sliderMoved(int position);

    void on_bottomPaddingText_editingFinished();

    void on_topPaddingText_editingFinished();

    void on_scroll_sliderMoved(int position);

    void updateMousePosition(QMouseEvent *event);
    void legendClick2(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *);

    void on_windowModeToggleButton_clicked();

private:
    void updateMouseTags();
    void graphAreaSetup(ParsedDataStorage* data);
    void UISetup();
    void graphSetup();
    void setGraphData();
    void updateAxisTags();
    void updateUIRanges();
    void plotGraph();

    int mouseX = 0, mouseY = 0;
    Ui::PlotAndDump *ui;
    QPointer<QCPGraph> graph[3];
    AxisTag* tag[3];
    AxisTag* mouseTag[3];
    const QIcon *toggleWindowModeIcon;

};

#endif // PLOTANDDUMP_H
