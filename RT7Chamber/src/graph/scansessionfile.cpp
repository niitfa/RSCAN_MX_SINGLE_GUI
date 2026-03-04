#include "scansessionfile.h"
#include <QDate>
#include <QTime>
#include <QtMath>
#include <QTextStream>
#include <iostream>

ScanSessionFile::ScanSessionFile()
{
}

ScanSessionFile::~ScanSessionFile()
{
}

bool ScanSessionFile::start()
{
    // create filepath
    this->filepath = this->getFolderPath();

    // open file, check if opened
    QString fname = this->filename;

    // check dir existing
    QDir dir;

    if(!dir.exists(this->filepath))
        dir.mkpath(this->filepath);

    file = new QFile(this->filepath + fname);
    if(file->open(QIODevice::ReadWrite))
    {
        printHeadText(file);
        printValuesDescription(this->file);
        return true;
    }
    return false;
}

void ScanSessionFile::update()
{
    if(this->file)
    {
        printReqularData(this->file);
    }
}

void ScanSessionFile::stop()
{
    // clear
    this->file->close();
    delete this->file;
    this->file = nullptr;
}

void ScanSessionFile::setFilename(QString filename)
{
    this->filename = filename;
}

void ScanSessionFile::setData(int index, uint16_t data)
{
    if(index >= 0 && index < this->vecSize)
    {
        this->vec[index] = data;
    }
}

void ScanSessionFile::setID(uint32_t id)
{
    this->id = id;
}

// private
QString ScanSessionFile::getFolderPath()
{
    QString path = QDir::currentPath();
    path += "/RSCAN_MX_SESSIONS/" + this->createCurrentTimeStr() + "_" + filename + "/";
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

void ScanSessionFile::printHeadText(QFile* f)
{
    QTextStream stream(f);
    stream << "Date," <<  getDateStringFile() << endl;
    stream << "Time," <<  getTimeStringFile() << endl;
    stream << "Size," <<  this->vecSize << endl;
    stream << endl;

}

void ScanSessionFile::printValuesDescription(QFile *f)
{
    QTextStream stream(f);
    stream << "Time," << "Date," << "ID,";

    for(int i = 0; i < this->vecSize; i++)
    {
        stream << QString::number(i / mxLineLength) + ":" +
                  QString::number(i % mxLineLength) + ",";
    }
    stream << endl;
}

void ScanSessionFile::printReqularData(QFile *f)
{
    QTextStream stream(f);

    // заполнение строки
    stream <<
              getTimeStringFile() << "," <<
              getDateStringFile() << "," <<
              QString::number(id) << ",";

    for(int i = 0; i < this->vecSize; i++)
    {
        stream << QString::number(this->vec.at(i)) + ",";
    }

    stream << endl;
}

QString ScanSessionFile::getTimeStringFile()
{
    QTime time = QTime::currentTime();

    QString str;
    str += this->QStringFromInt(time.hour(), 2);
    str += ":";
    str += this->QStringFromInt(time.minute(), 2);
    str += ":";
    str += this->QStringFromInt(time.second(), 2);
    str += ".";
    str += this->QStringFromInt(time.msec(), 3);
    return str;
}

QString ScanSessionFile::getDateStringFile()
{
    QDate date = QDate::currentDate();

    QString str;
    str += this->QStringFromInt(date.day(), 2);
    str += ".";
    str += this->QStringFromInt(date.month(), 2);
    str += ".";
    str += this->QStringFromInt(date.year(), 4);
    return str;
}
