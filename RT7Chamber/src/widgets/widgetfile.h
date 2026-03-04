#ifndef WIDGETFILE_H
#define WIDGETFILE_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include "scansessionfile.h"

namespace Ui {
class WidgetFile;
}

class WidgetFile : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFile(QWidget *parent = nullptr);
    ~WidgetFile();
    void setSession(ScanSessionFile*);
    ScanSessionFile *getSession();
    void update(uint32_t id);

private slots:
    void on_lineEdit_periodInput_editingFinished();
    void on_pushButton_start_clicked();

private:
    void setStartStyle(QPushButton*);
    void setStopStyle(QPushButton*);

private:
    bool started = 0;
    Ui::WidgetFile *ui;
    QFont buttonsFont;
    uint32_t fileUpdatePeriod = 1;
    uint32_t pointIndex = 0;
    ScanSessionFile* session = nullptr;
    QString defaultFilename = "session";
};

#endif // WIDGETFILE_H
