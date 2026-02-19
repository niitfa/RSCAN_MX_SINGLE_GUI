#ifndef SCANSESSIONFILE_H
#define SCANSESSIONFILE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QFile>

class ScanSessionFile
{
    QString filepath;
    QFile* file = nullptr;
public:
    ScanSessionFile();

    bool start();
    void update(QVector<int>);
    void stop();

private:
    QString getFolderPath();
    QString createCurrentTimeStr();
    QString QStringFromInt(int num, int numMinimumLength);
};

#endif // SCANSESSIONFILE_H
