#include "scansessionfile.h"
#include <QDate>
#include <QTime>
#include <QtMath>
#include <QTextStream>
#include <iostream>

ScanSessionFile::ScanSessionFile()
{
}

bool ScanSessionFile::start()
{
    // create filepath
    this->filepath = this->getFolderPath();

    // open file, check if opened
    QString fname = "session.csv";

    // check dir existing
    QDir dir;

    if(!dir.exists(this->filepath))
        dir.mkpath(this->filepath);

    file = new QFile(this->filepath + fname);
    if(file->open(QIODevice::ReadWrite))
    {
        QTextStream stream(file);
        //std::cout << "opened\n";
        return true;
    }
    //std::cout << "error opening\n";
    return false;
}

void ScanSessionFile::update(QVector<int> data)
{
    if(this->file)
    {
        if(!data.empty())
        {
            // common file
            QTextStream stream(this->file);
            for(auto val : data)
            {
                stream << val << ",";
            }
            stream << endl;

            // single file
            QDir pointsDir;
            QString pointsFilePath = this->filepath + "points/";
            if(!pointsDir.exists(pointsFilePath))
                pointsDir.mkpath(pointsFilePath);

            QFile pointFile(pointsFilePath + QString::number(data[0]) + ".csv");
            if(pointFile.open(QIODevice::ReadWrite))
            {
                QTextStream pointStream(&pointFile);
                for(auto val : data)
                {
                    pointStream << val << ",";
                }
                stream << endl;
                pointFile.close();
            }
        }
    }
}

void ScanSessionFile::stop()
{
    // clear
    this->file->close();
    delete this->file;
    this->file = nullptr;
}


// private
QString ScanSessionFile::getFolderPath()
{
    QString path = QDir::currentPath();
    //while(!path.endsWith('/')) { path.chop(1); }
    path += "/RSCAN_MX_V2_SESSIONS/" + this->createCurrentTimeStr() + "/";
    return path;
}

QString ScanSessionFile::createCurrentTimeStr()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    QString str;
    str += this->QStringFromInt(date.year(), 4);
    str += "-";
    str += this->QStringFromInt(date.month(), 2);
    str += "-";
    str += this->QStringFromInt(date.day(), 2);
    str += "-";
    str += this->QStringFromInt(time.hour(), 2);
    str += "-";
    str += this->QStringFromInt(time.minute(), 2);
    str += "-";
    str += this->QStringFromInt(time.second(), 2);
    return str;
}

QString ScanSessionFile::QStringFromInt(int num, int numMinimumLength)
{
    if(numMinimumLength)
    {
        int numCopy = num;
        int numLength = 1;
        while((numCopy /= 10)) { numLength++; }
        numLength = qMax(numLength, numMinimumLength);

        QString res;
        int div = static_cast<int>(qPow(10, numLength - 1));
        for(int i = 0; i < numLength; i++)
        {
            int digit = num / div;
            res += QString::number(digit);
            num %= div;
            div /= 10;
        }
        return res;
    }
    return QString::number(num);
}
