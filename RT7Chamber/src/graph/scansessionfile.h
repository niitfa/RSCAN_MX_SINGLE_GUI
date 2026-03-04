#ifndef SCANSESSIONFILE_H
#define SCANSESSIONFILE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QVector>

class ScanSessionFile
{
    QString filepath;
    QFile* file = nullptr;

    // special - regular
    uint32_t  id = 0;

    // other
    QString filename = "rscan_mx";
    QVector<uint16_t> vec = QVector<uint16_t>(vecSize);

public:
    ScanSessionFile();
    ~ScanSessionFile();

    static const int vecSize = 16;
    static const int mxLineLength = 4;

    bool start();
    void update();
    void stop();

    void setFilename(QString);
    void setData(int index, uint16_t data);
    // title

    // regular
    void setID(uint32_t id);

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
