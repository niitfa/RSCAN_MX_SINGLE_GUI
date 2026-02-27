#ifndef SCANSESSIONFILE_H
#define SCANSESSIONFILE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>

class ScanSessionFile
{
    QString filepath;
    QFile* file = nullptr;

    // special - title
    double BqPerCount   = 1;
    double noiseMBq     = 0;

    // special - regular
    int         id                      = 0;
    int         rawActivityCount        = 0;
    double      rawActivityMBq          = 0;
    double      noiselessActivityMBq    = 0;
    uint8_t     sensitivity             = 0;
    uint16_t    voltage                 = 0;
    uint8_t     voltagePolarity         = 0;
    double      pressureAt              = 0;

    QString filename = "rwell";
public:
    ScanSessionFile();
    ~ScanSessionFile();

    bool start();
    void update();
    void stop();

    void setFilename(QString);

    // title
    void setBqPerCount(double BqPerCount);
    void setNoiseMBq(double noiseMBq);

    // regular
    void setID(int id);
    void setRawActivityCount(int cnt);
    void setRawActivityMBq(double MBq);
    void setNoiselessActivityMBq(double MBq);
    void setSensitivity(uint8_t sensitivity);
    void setVoltage(uint16_t voltage);
    void setVoltagePolarity(uint8_t polarity);
    void setPressure(double pressureAt);

private:
    QString getFolderPath();
    QString createCurrentTimeStr();
    QString QStringFromInt(int num, int numMinimumLength);
    void printHeadText(QFile* f);
    void printValuesDescription(QFile* f);
    void printReqularData(QFile* f);
    QString getTimeStringFile();
    QString getDateStringFile();

};

#endif // SCANSESSIONFILE_H
