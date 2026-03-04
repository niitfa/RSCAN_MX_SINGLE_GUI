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
#include "widgetvalue.h"

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

    void on_checkBox_allGraphs_clicked();

private:
    void setStartStyle(QPushButton*);
    void setStopStyle(QPushButton*);

    void initWidgetValues();
    void updateWidgetValues();
private:

    RSCANMXDev2Receiver* receiver = nullptr;
    Ui::ChamberWindow *ui;
    QTimer* timer = nullptr;

    uint32_t id = 0;

    // Graph
    QGraph* graph = nullptr;
    double yGraphMaxRange = 70000; // was 9000000
    double yGraphMinRange = -5000;
    double tGraphRange = 20;
    QVector<double> vecCells;

    // write to file
    ScanSessionFile session;

    // start graph button state
    bool graphStarted = 1;

    // font
    QFont buttonsFont;

    // Widgets value
    QVector<WidgetValue*> widgetValues;
private:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
    void logFileUpdate();
};

#endif // CHAMBERWINDOW_H
