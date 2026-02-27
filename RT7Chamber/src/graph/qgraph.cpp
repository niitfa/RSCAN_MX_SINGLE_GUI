#include "qgraph.h"
#include "ui_qgraph.h"
#include <iostream>

QGraph::QGraph(QWidget *parent) :
    QCustomPlot(parent),
    ui(new Ui::QGraph)
{
    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showPointToolTip(QMouseEvent*)));
    ui->setupUi(this);
    this->QCustomPlot::xAxis->setLabel("Время, c");
    this->QCustomPlot::yAxis->setLabel("Активность, МБк");
    //this->QCustomPlot::yAxis2->setVisible(true);
    //this->QCustomPlot::yAxis2->setLabel("ADC output, cnt");
    this->setupGraph();
    //this->setBackground(QColor(200, 200, 200, 0));
}

QGraph::~QGraph()
{
    delete ui;
}

double QGraph::back()
{
    if(this->yVec.isEmpty())
    {
        return 0;
    }
    return this->yVec.back();
}

void QGraph::show()
{
    int width = parentWidget()->geometry().width();
    int height = parentWidget()->geometry().height();
    this->setGeometry(0, 0, width, height);
    this->QCustomPlot::show();
}

#include <iostream>
void QGraph::update(double val)
{
    if(this->enabled)
    {
        // get current timе
        int msecs_elapsed = timer.restart();
        double secs_elapsed = static_cast<double>(msecs_elapsed) / 1000;

        this->updateTimeVector(secs_elapsed);
        this->yVec.push_back((val));

        while(this->isLimitTimeExceeded())
        {
            tMin += secs_elapsed;
            tMax += secs_elapsed;
            this->tVec.pop_front();
            this->yVec.pop_front();
            this->setTAxisRange(tMin, tMax);
        }

        this->QGraph::replot();
    }
}

void QGraph::resizeYAxis()
{
    // max yVec, min yVec
}

void QGraph::setYAxisRange(double yMin, double yMax)
{
    this->yMin = yMin;
    this->yMax = yMax;
    this->QCustomPlot::yAxis->setRange(this->yMin, this->yMax);
    this->QGraph::replot();
}

void QGraph::setTAxisRange(double tMin, double tMax)
{
    this->tMin = tMin;
    this->tMax = tMax;
    this->QCustomPlot::xAxis->setRange(this->tMin, this->tMax);
    this->QGraph::replot();
}

void QGraph::updateTimeVector(double t)
{
    if(tVec.empty())
    {
        tVec.push_back(t);
    }
    else
    {
        tVec.push_back(t + this->tVec.back());
    }
}

bool QGraph::isLimitTimeExceeded()
{
    double limitTime = this->tMin + this->tFilled * (this->tMax - this->tMin);
    return this->tVec.back() > limitTime;
}

void QGraph::setEnabled(bool enabled)
{
    if(enabled)
    {
        if(!this->enabled)
        {
            this->resetData();
            this->removeGraph();
            this->setupGraph();
        }
    }
    this->enabled = enabled;
}

double QGraph::getTimeRange()
{
    return this->tMax - this->tMin;
}

double QGraph::getYMax()
{
    return this->yMax;
}

double QGraph::getYMin()
{
    return this->yMin;
}

void QGraph::resetData()
{
    this->tVec.clear();
    this->yVec.clear();
    double tAxisRange = this->tMax - this->tMin;
    this->tMin = 0;
    this->tMax = tAxisRange;
    timer.restart();
    this->setTAxisRange(tMin, tMax);
}

void QGraph::setupGraph()
{
    if(!this->QCustomPlot::graphCount())
    {
        this->QCustomPlot::addGraph();
        QPen pen;
        QColor graphColor;
        graphColor.setRgb(0, 100, 0);
        pen.setColor(graphColor);
        pen.setWidth(3);
        this->QCustomPlot::graph(0)->setPen(pen);
    }
}

void QGraph::removeGraph()
{
    if(this->QCustomPlot::graphCount() == 1)
    {
        this->QCustomPlot::removeGraph(0);
    }
}

void QGraph::replot()
{
    if(this->QCustomPlot::graphCount() == 1)
    {
        this->QCustomPlot::graph(0)->data()->clear(); // !!
        this->QCustomPlot::graph(0)->addData(this->tVec, this->yVec, true);
        this->QCustomPlot::replot();
    }
}

void QGraph::clearGraph()
{
    if(this->QCustomPlot::graphCount() == 1)
    {
        this->QCustomPlot::graph(0)->data()->clear();
        this->QCustomPlot::replot();
    }
}

//#include <iostream>
void QGraph::showPointToolTip(QMouseEvent * event)
{
   // std::cout << "selected: " << QCPGraph::selectTest() << std::endl;
    if(1)
    {
        double x = this->xAxis->pixelToCoord(event->pos().x());
        double y = this->yAxis->pixelToCoord(event->pos().y());

        // find nearest index
        if(this->tVec.size() > 1)
        {
            if(x < this->tVec.front() || x > this->tVec.back()) { return; }

            double x_base = this->tVec[0];
            //double x_delta = this->tVec[1] - this->tVec[0];
            double x_delta = (this->tVec.back() - this->tVec.front())/this->tVec.size();

            int index = static_cast<int>(floor((x - x_base) / x_delta));
           // int index = static_cast<int>(floor((x - x_base) / x_delta));

            if(index < this->yVec.size())
            {
                double x_show = this->tVec[index];//x_base + index * x_delta;
                double y_show = this->yVec[index];
                setToolTip(QString("%1 с, %2 МБк").arg(x_show).arg(y_show));
            }
        }
    }
}
