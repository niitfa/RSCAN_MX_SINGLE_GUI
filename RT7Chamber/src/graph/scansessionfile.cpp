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

void ScanSessionFile::setBqPerCount(double BqPerCount)
{
    this->BqPerCount = BqPerCount;
}

void ScanSessionFile::setNoiseMBq(double noiseMBq)
{
    this->noiseMBq = noiseMBq;
}

void ScanSessionFile::setID(int id)
{
    this->id = id;
}

void ScanSessionFile::setRawActivityCount(int cnt)
{
    this->rawActivityCount = cnt;
}

void ScanSessionFile::setRawActivityMBq(double MBq)
{
    this->rawActivityMBq = MBq;
}

void ScanSessionFile::setNoiselessActivityMBq(double MBq)
{
    this->noiselessActivityMBq = MBq;
}

void ScanSessionFile::setSensitivity(uint8_t sensitivity)
{
    this->sensitivity = sensitivity;
}

void ScanSessionFile::setVoltage(uint16_t voltage)
{
    this->voltage = voltage;
}

void ScanSessionFile::setVoltagePolarity(uint8_t polarity)
{
    this->voltagePolarity = polarity;
}

void ScanSessionFile::setPressure(double pressureAt)
{
    this->pressureAt = pressureAt;
}

// private
QString ScanSessionFile::getFolderPath()
{
    QString path = QDir::currentPath();
    //while(!path.endsWith('/')) { path.chop(1); }
    //path += "/RWELL_SESSIONS/" + this->createCurrentTimeStr() + "/";
    path += "/RWELL_SESSIONS/" + this->createCurrentTimeStr() + "/";
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
    stream << "Background (MBq)," <<  QString::number(noiseMBq) << endl;
    stream << "Voltage (V)," <<  QString::number(voltage) << endl;
    stream << "Polarity," <<  (voltagePolarity ? "+" : "-") << endl;
    stream << "Sensitivity," <<  (sensitivity ? "low" : "high") << endl;
    stream << endl;

}

void ScanSessionFile::printValuesDescription(QFile *f)
{
    QTextStream stream(f);
    stream <<
            "Time," <<
            "Date," <<
            "ID," <<
            "Activity (count)," <<
            "Activity (MBq)," <<
            "Activity no BG (MBq)," <<
            "Voltage (V)," <<
            "Pressure (at)," <<
            "Polarity," <<
            "Sensitivity" << endl;
}

void ScanSessionFile::printReqularData(QFile *f)
{
    QTextStream stream(f);
    // заполнение строки
    stream <<
              getTimeStringFile() << "," <<
              getDateStringFile() << "," <<
              QString::number(id) << "," <<
              QString::number(rawActivityCount) << "," <<
              QString::number(rawActivityMBq, 'f', 2) << "," <<
              QString::number(noiselessActivityMBq, 'f', 2) << "," <<
              QString::number(voltage) << "," <<
              QString::number(pressureAt, 'f', 2) << "," <<
              (voltagePolarity ? "+" : "-") << "," <<
              (sensitivity ? "low" : "high") << endl;
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
