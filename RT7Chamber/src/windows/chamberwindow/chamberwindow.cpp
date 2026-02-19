#include "chamberwindow.h"
#include "ui_chamberwindow.h"
#include <QPalette>
#include <iostream>
#include <QtMath>

// 1) ChamberWindow::ChamberWindow(...) int interval = 10
// 2) ChamberWindow::update(...) if(...)
// 3) ChamberWindow::update(...) emul values
// 3) ChamberWindow::update(...) id++ emul

ChamberWindow::ChamberWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChamberWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("RSCAN MX - Single cell");

    //init timer
    int interval = 10; // was 10

    timer = new QTimer( this );
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->setSingleShot(false);
    timer->setInterval(interval);

    //set readonly
    QPalette greyPalette;
    greyPalette.setColor(QPalette::Base, QColor(235, 235, 235));

    // MCU connection
    this->receiver = new RSCANMXDev2Receiver();

    // Init graph
    this->graph = new QGraph(ui->widget_graph);
    //this->graph->setNanoamperPerCount(this->kBqPerCount_coarse * 1e-6); // MBq per count!!
    this->graph->setNanoamperPerCount(1); // raw counts
    this->graph->resetNoise();
    this->graph->setTAxisRange(0, static_cast<double>(this->tGraphRange));
    this->graph->setYAxisRange(this->yGraphMinRange, this->yGraphMaxRange);

    // line edits
    ui->lineEdit_graphVerticalMax->setText(QString::number(this->yGraphMaxRange));
    ui->lineEdit_graphVerticalMin->setText(QString::number(this->yGraphMinRange));
    ui->lineEdit_graphHorizontalRange->setText(QString::number(this->tGraphRange));
    ui->lineEdit_cellX->setText(QString::number(this->cellX));
    ui->lineEdit_cellY->setText(QString::number(this->cellY));
    ui->lineEdit_kNanoamperPerCount->setText(QString::number(this->kNanoAmperPerCount));
    ui->widget_fileMenu->setSession(&this->session);

    // start graph button
    this->graphStarted = 1;
    this->setStopStyle(ui->pushButton_startGraph);

    // value widgets
    ui->widget_currentActivity->setHeadText("Сигнал ячейки, ед.:");
    ui->widget_averageActivity->setHeadText("Фон ячейки, ед.:");
    ui->widget_current->setHeadText("Ток, нА:");
    ui->widget_noiseCurrent->setHeadText("Фоновый ток, нА:");

    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    // buttons
    ui->pushButton_compensateBG->hide();
    //ui->pushButton_compensateBG->setFont(buttonsFont);
    //ui->pushButton_compensateBG->setText("Вычесть фон");
    ui->pushButton_resetBG->hide();

    // check box
    ui->checkBox_noise->setFont(buttonsFont);
    ui->checkBox_noise->setText("Вычесть фон");

    // set fonts
    ui->pushButton_compensateBG->setFont(buttonsFont);
    ui->pushButton_resetBG->setFont(buttonsFont);
    ui->pushButton_resetScales->setFont(buttonsFont);
    ui->pushButton_startGraph->setFont(buttonsFont);
    ui->label->setFont(buttonsFont);
    ui->label_2->setFont(buttonsFont);
    ui->label_16->setFont(buttonsFont);
    ui->label_cellX->setFont(buttonsFont);
    ui->label_cellY->setFont(buttonsFont);
    ui->label_kNanoamperPerCount->setFont(buttonsFont);
    ui->lineEdit_graphHorizontalRange->setFont(buttonsFont);
    ui->lineEdit_graphVerticalMax->setFont(buttonsFont);
    ui->lineEdit_graphVerticalMin->setFont(buttonsFont);
    ui->lineEdit_cellX->setFont(buttonsFont);
    ui->lineEdit_cellY->setFont(buttonsFont);
    ui->lineEdit_kNanoamperPerCount->setFont(buttonsFont);

}

ChamberWindow::~ChamberWindow()
{
    delete ui;
    delete receiver;
    delete graph;
}

void ChamberWindow::connect(std::string ip, uint16_t outputPort, uint16_t inputPort)
{
    if(this->receiver)
    {
        receiver->conn(ip, outputPort);
    }
}

void ChamberWindow::disconnect()
{
    if(this->receiver)
    {
        receiver->disconn();
    }
}

bool ChamberWindow::isConnected()
{
    bool receverConnected = true;
    if(this->receiver)
    {
        receverConnected = receiver->isConnected();
    }

    return (receverConnected);
}

void ChamberWindow::show()
{
    this->QDialog::show();
    this->graph->show();
    if(!timer->isActive())
    {
        timer->start();
    }

}

void ChamberWindow::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
    this->ChamberWindow::disconnect();

}
void ChamberWindow::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    show();
}

void ChamberWindow::update()
{
    // update message from MCU
    if(this->id < receiver->getMessageID())
    {
        id = receiver->getMessageID();
        int currDoseRate = receiver->getDetectorValue(this->cellX, this->cellY);

        ui->widget_currentActivity->setValueText(QString::number(currDoseRate - this->graph->getNoiseCount()));
        ui->widget_averageActivity->setValueText(QString::number(this->graph->getNoiseCount()));
        ui->widget_noiseCurrent->setValueText(QString::number(
                                                  this->graph->getNoiseCount() * this->kNanoAmperPerCount, 'f', 6)
                                              );
        ui->widget_current->setValueText(
                    QString::number(((currDoseRate - this->graph->getNoiseCount())* this->kNanoAmperPerCount), 'f', 6)
                    );

        // graph update
        if(this->graph)
        {
            this->graph->QGraph::updateCount(currDoseRate);
        }
        // file update
        ui->widget_fileMenu->update(id, currDoseRate);
    }
}

void ChamberWindow::on_pushButton_startGraph_clicked()
{
    if(this->graphStarted)
    {
        if(this->graph)
        {
            this->graph->setEnabled(false);
            this->setStartStyle(ui->pushButton_startGraph);
        }
    }
    else
    {
        if(this->graph)
        {
            this->graph->setEnabled(true);
            this->setStopStyle(ui->pushButton_startGraph);
        }
    }
    this->graphStarted = !this->graphStarted;
}

void ChamberWindow::on_lineEdit_graphHorizontalRange_editingFinished()
{
    double tRange = ui->lineEdit_graphHorizontalRange->text().toDouble();
    if(tRange > 0)
    {
        this->graph->setEnabled(false);
        this->graph->setTAxisRange(0, tRange);
    }
    else
    {
         ui->lineEdit_graphHorizontalRange->setText(
                     QString::fromStdString(std::to_string(this->graph->getTimeRange()))
                     );
    }
}

void ChamberWindow::on_lineEdit_graphVerticalMin_editingFinished()
{
    double min = ui->lineEdit_graphVerticalMin->text().toDouble();
    double currMax = this->graph->getYMax();
    if(min < currMax)
    {
        this->graph->setYAxisRange(min, currMax);
    }
    else
    {
        int currMinInt = static_cast<int>(this->graph->getYMin());
        ui->lineEdit_graphVerticalMin->setText(
                    QString::fromStdString(std::to_string(currMinInt))
                    );
    }
}

void ChamberWindow::on_lineEdit_graphVerticalMax_editingFinished()
{
    double max = ui->lineEdit_graphVerticalMax->text().toDouble();
    double currMin = this->graph->getYMin();
    if(max > currMin)
    {
        this->graph->setYAxisRange(currMin, max);
    }
    else
    {
        int currMaxInt = static_cast<int>(this->graph->getYMax());
        ui->lineEdit_graphVerticalMax->setText(
                    QString::fromStdString(std::to_string(currMaxInt))
                    );
    }
}

void ChamberWindow::on_pushButton_resetScales_clicked()
{
    this->graph->setYAxisRange(this->yGraphMinRange, this->yGraphMaxRange);
    ui->lineEdit_graphVerticalMax->setText(QString::fromStdString(std::to_string(static_cast<int>(this->yGraphMaxRange))));
    ui->lineEdit_graphVerticalMin->setText(QString::fromStdString(std::to_string(static_cast<int>(this->yGraphMinRange))));
}

void ChamberWindow::on_pushButton_compensateBG_clicked()
{
    this->graph->updateNoise();
    /*if(receiver)
    {
        this->averageDoseCountSaved = receiver->GetADCAverageValue();
    } */
}

void ChamberWindow::on_pushButton_resetBG_clicked()
{
    this->graph->resetNoise();
}

void ChamberWindow::setStartStyle(QPushButton * button)
{
    QIcon icon(":/img/button_icon_start.png");
    int iconSize = 14;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText("Возобновить");

    button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"
                "QPushButton { color: white; }"
                // hover
                "QPushButton:hover { background-color: rgb(40,100,200);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(30,80,170);  }"
                ); // Start */
    button->setFocusPolicy( Qt::FocusPolicy::NoFocus );

}

void ChamberWindow::setStopStyle(QPushButton * button)
{
    QIcon icon(":/img/button_icon_pause.png");
    int iconSize = 24;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText("Остановить");

    button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"
                "QPushButton { color: white; }"
                // hover
                "QPushButton:hover { background-color: rgb(40,100,200);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(30,80,170);  }"
                ); // Start */
    button->setFocusPolicy( Qt::FocusPolicy::NoFocus );
}

void ChamberWindow::on_checkBox_noise_clicked()
{
    if(ui->checkBox_noise->checkState())
    {
        this->graph->updateNoise();
    }
    else
    {
        this->graph->resetNoise();
    }
}

void ChamberWindow::on_lineEdit_cellX_editingFinished()
{
    int num = ui->lineEdit_cellX->text().toInt();
    if(num > 0 || num < RSCANMXDev2Receiver::kLineLength)
    {
        this->cellX = num;
    }
    else if(num == 0)
    {
        this->cellX = num;
        ui->lineEdit_cellX->setText(QString::number(num));
    }
    else
    {
        ui->lineEdit_cellX->setText(QString::number(this->cellX));
    }
}

void ChamberWindow::on_lineEdit_cellY_editingFinished()
{
    int num = ui->lineEdit_cellY->text().toInt();
    if(num > 0 || num < RSCANMXDev2Receiver::kNumberOfLines)
    {
        this->cellY = num;
    }
    else if(num == 0)
    {
        this->cellY = num;
        ui->lineEdit_cellY->setText(QString::number(num));
    }
    else
    {
        ui->lineEdit_cellY->setText(QString::number(this->cellY));
    }

}

void ChamberWindow::on_lineEdit_kNanoamperPerCount_editingFinished()
{
    double val = ui->lineEdit_kNanoamperPerCount->text().toDouble();
    if(val != 0)
    {
        this->kNanoAmperPerCount = val;
    }
    else
    {
        ui->lineEdit_kNanoamperPerCount->setText(QString::number(this->kNanoAmperPerCount));
    }
}
