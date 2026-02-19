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
    void update(int id, int value);

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
    int fileUpdatePeriod = 1200;
    int pointIndex = 0;
    ScanSessionFile* session = nullptr;
};

#endif // WIDGETFILE_H
