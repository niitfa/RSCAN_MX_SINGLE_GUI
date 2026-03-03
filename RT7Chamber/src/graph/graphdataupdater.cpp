#include "graphdataupdater.h"

GraphDataUpdater::GraphDataUpdater(QVector<QVector<double>>* vec)
{
    if(vec)
    {
        this->pvec = vec;
        this->size = pvec->size();
        this->threads = new std::thread[pvec->size()];
    }
}

GraphDataUpdater::~GraphDataUpdater()
{
    delete[] this->threads;
}

void GraphDataUpdater::push(const QVector<double>& data)
{
    int imax = qMin(this->size, this->pvec->size());
    for(int i = 0; i < imax; i++)
    {
        threads[i] = std::thread(
                    &GraphDataUpdater::pushHandler, this, i, data.at(i)
                    );
    }

    for(int i = 0; i < imax; i++)
    {
        threads[i].join();
    }

}

void GraphDataUpdater::pop()
{
    int imax = qMin(this->size, this->pvec->size());
    for(int i = 0; i < imax; i++)
    {
        threads[i] = std::thread(
                    &GraphDataUpdater::popHandler, this, i
                    );
    }

    for(int i = 0; i < imax; i++)
    {
        threads[i].join();
    }
}

void GraphDataUpdater::pushHandler(int vecIndex, double val)
{
    // add checks!!!
    if(this->pvec)
    {
        (*this->pvec)[vecIndex].push_back(val);
    }
}

void GraphDataUpdater::popHandler(int vecIndex)
{
    if(this->pvec)
    {
        (*this->pvec)[vecIndex].pop_front();
    }
}
