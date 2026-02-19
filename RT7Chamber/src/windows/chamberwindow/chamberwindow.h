#ifndef CHAMBERWINDOW_H
#define CHAMBERWINDOW_H

#include <QDialog>
#include <QTimer>
#include <QtMath>
#include <QFont>
#include <QString>
#include "qgraph.h"
#include "rscanmxdev2receiver.h"
#include "scansessionfile.h"

namespace Ui {
class ChamberWindow;
}

class ChamberWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChamberWindow(QWidget *parent = nullptr);
    ~ChamberWindow() override;

    void connect(std::string ip, uint16_t outputPort, uint16_t inputPort);
    void disconnect();
    bool isConnected();
    void show();

private slots:
    void update();

    void on_pushButton_startGraph_clicked();

    void on_lineEdit_graphHorizontalRange_editingFinished();

    void on_lineEdit_graphVerticalMin_editingFinished();

    void on_lineEdit_graphVerticalMax_editingFinished();

    void on_pushButton_resetScales_clicked();

    void on_pushButton_compensateBG_clicked();

    void on_pushButton_resetBG_clicked();
    void on_checkBox_noise_clicked();

    void on_lineEdit_cellX_editingFinished();

    void on_lineEdit_cellY_editingFinished();

    void on_lineEdit_kNanoamperPerCount_editingFinished();

private:
    void setStartStyle(QPushButton*);
    void setStopStyle(QPushButton*);
private:

    RSCANMXDev2Receiver* receiver = nullptr;
    Ui::ChamberWindow *ui;
    QTimer* timer = nullptr;

    int id = 0;

    // Graph
    QGraph* graph = nullptr;
    double yGraphMaxRange = 70000; // was 9000000
    double yGraphMinRange = -5000;
    double tGraphRange = 20;
    const int maxVoltage = 500;

    // MBq per count
    double kBqPerCount_coarse = 13000;
    double kSense = 1;
    double kBqPerCount_fine = kBqPerCount_coarse / kSense;
    double kBqPerCountCurrent = kBqPerCount_coarse;
    int prevRange = 2; // nor 0 not 1

    // nA per count
    double kNanoAmperPerCount = 4e-8;

    // Matrix!!
    int cellX = 0;
    int cellY = 0;

    // write to file
    ScanSessionFile session;

    // start graph button state
    bool graphStarted = 1;

    // font
    QFont buttonsFont;

    // average activity
    int averageDoseCountSaved = 0;
private:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
};

#endif // CHAMBERWINDOW_H
