#ifndef GRAPHDATAUPDATER_H
#define GRAPHDATAUPDATER_H

#include <thread>
#include <QVector>

class GraphDataUpdater
{
    int size = 0;
    std::thread* threads = nullptr;
    QVector<QVector<double>>* pvec = nullptr;
public:
    GraphDataUpdater(QVector<QVector<double>>* vec);
    ~GraphDataUpdater();
    void push(const QVector<double>& data);
    void pop();
private:
    void pushHandler(int vecIndex, double val);
    void popHandler(int vecIndex);
};

#endif // GRAPHDATAUPDATER_H
