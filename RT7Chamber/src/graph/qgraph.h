#ifndef QGRAPH_H
#define QGRAPH_H

#include <QWidget>
#include <QtMath>
#include <QVector>
#include <QTime>
#include "qcustomplot.h"

namespace Ui {
class QGraph;
}

class QGraph : public QCustomPlot
{
    Q_OBJECT

public:
    explicit QGraph(QWidget *parent = nullptr);
    ~QGraph();

    void setNanoamperPerCount(double);
    double getNanoamperPerCount();

    //void setNanoamperPerCount(double);
    //double getNanoamperPerCount();

    void updateNoise();
    void resetNoise();
    int getNoiseCount();
    double getNoiseNanoamper();
    double back();

    void setYAxisRange(double yMix, double yMax);
    void setTAxisRange(double tMin, double tMax);

    void show();
    void updateCount(int val);
    void resizeYAxis();
    void setEnabled(bool);
    double getTimeRange();
    double getYMax();
    double getYMin();
private:
    Ui::QGraph *ui;
    QTime timer;
    bool enabled = true;
    double kNanoamperPerCount = 4e-8;
    int noiseCount = 0;
    double noiseNanoamper = 0;

    // MBq per count

    QVector<double> yVec;
    QVector<double> tVec;

    // t axis
    double tFilled = 0.75; // (0, 1]
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
private slots:
    void showPointToolTip(QMouseEvent*);
};

#endif // QGRAPH_H
