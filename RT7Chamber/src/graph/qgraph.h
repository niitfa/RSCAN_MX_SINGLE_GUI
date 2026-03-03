#ifndef QGRAPH_H
#define QGRAPH_H

#include <QWidget>
#include <QtMath>
#include <QVector>
#include <QTime>
#include "qcustomplot.h"
#include <thread>
#include "graphdataupdater.h"

namespace Ui {
class QGraph;
}

class QGraph : public QCustomPlot
{
    Q_OBJECT

public:
    explicit QGraph(QWidget *parent = nullptr);
    ~QGraph();

    QVector<double> back();
    void setYAxisRange(double yMix, double yMax);
    void setTAxisRange(double tMin, double tMax);
    void show();
    void update(const QVector<double>& data);
    void resizeYAxis();
    void setEnabled(bool);
    double getTimeRange();
    double getYMax();
    double getYMin();

    void showGraph(int index);
    void hideGraph(int index);
    void showAllGraphs();
    void hideAllGraphs();

    static const int numberOfGraphs = 16;
private:
    QVector<QCPPlottableLegendItem> savedLegendItems;

    Ui::QGraph *ui;
    QTime timer;
    bool enabled = true;

    QVector<QVector<double>> yVec;
    QVector<double> tVec;
    GraphDataUpdater* dataUpdater = nullptr;

    // t axis
    double tFilled = 0.85; // (0, 1]
    double tMin = 0;
    double tMax = 1;

    // y axis
    double yMin = 0;
    double yMax = 100;
private:
    void updateTimeVector(double t);
    bool isLimitTimeExceeded();
    void resetData();
    void setupGraph();
    void removeGraph();
    void replot();
    void clearGraph();

    // vector op
    void yVecPushBack(const QVector<double>&);
    void yVecPopFront();
    void yVecClear();

    // legend
    void updateLegend();

    // thread
private slots:
    void showPointToolTip(QMouseEvent*);
};

#endif // QGRAPH_H
