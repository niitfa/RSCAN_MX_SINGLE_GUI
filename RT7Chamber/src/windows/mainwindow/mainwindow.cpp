#include <QMessageBox>
#include <QHostAddress>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "string_validator.h"
#include <string>
#include <iostream>

// 1 edit void MainWindow::on_pushButton_Connect_clicked() - emul
// 2 edit MainWindow::on_pushButton_Connect_clicked() ip string parsing

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("RSCAN MX - Single cell");
    setFixedSize(500,340);

    // fonts
    QFont font;
    font.setFamily("Inter");
    font.setPixelSize(16);

    // head icon
    int iconSize = 32;
    ui->label_headIcon->clear();
    ui->label_headIcon->setPixmap(QIcon(":/img/button_icon_settings.png").pixmap(iconSize, iconSize));

    // list font
    ui->comboBox_TypeSelect->setFont(font);

    // line edit fonts
    ui->lineEdit_ip->setFont(font);
    ui->lineEdit_InputPort->setFont(font);
    ui->lineEdit_OutputPort->setFont(font);

    // head text
    ui->label_headText->clear();
    ui->label_headText->setText("Камера и подключение");
    ui->label_headText->setFont(font);

    // border
    ui->frame_border->setStyleSheet(
                " QFrame { background-color: white; } "
                " QFrame { border-radius:20px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->gridLayout_frame->setParent(nullptr);
    ui->frame_border->setLayout(ui->gridLayout_frame);

    // label styles
    QString textColor = "color: rgb(120,120,120)";
    ui->label_deviceType->setStyleSheet(
                "border-width:0px;" +
                textColor
                ); // Тип устройства
    ui->label_deviceType->setFont(font);

    ui->label_ip->setStyleSheet(
                "border-width:0px;" +
                textColor
                ); // IP-адрес
     ui->label_ip->setFont(font);


    ui->label_inputPort->setStyleSheet(
                "border-width:0px;" +
                textColor
                ); // Входящий порт
    ui->label_inputPort->setFont(font);

    ui->label_outputPort->setStyleSheet(
                "border-width:0px;" +
                textColor
                ); // Исходящий порт
    ui->label_outputPort->setFont(font);

    // button style
    ui->pushButton_Connect->setStyleSheet(
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
                ); // Подключиться
    ui->pushButton_Connect->setFont(font);

    // fill combo box
    ui->comboBox_TypeSelect->addItem("Матрица");


    // default connection values
    ui->lineEdit_ip->setText(default_ip);
    ui->lineEdit_InputPort->setText(QString::number(default_inputPort));
    ui->lineEdit_OutputPort->setText(QString::number(default_outputPort));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete chamber_ui;
}

void MainWindow::on_lineEdit_InputPort_editingFinished()
{
    ui->lineEdit_InputPort->setText(
                StringValidator::ValidatePort(ui->lineEdit_InputPort->text())
                );
}

void MainWindow::on_lineEdit_OutputPort_editingFinished()
{
    ui->lineEdit_OutputPort->setText(
                StringValidator::ValidatePort(ui->lineEdit_OutputPort->text())
                );
}

void MainWindow::on_pushButton_Connect_clicked()
{
    std::string ip = ui->lineEdit_ip->text().toStdString();
    uint16_t inputPort = static_cast<uint16_t>(ui->lineEdit_InputPort->text().toInt());
    uint16_t outputPort = static_cast<uint16_t>(ui->lineEdit_OutputPort->text().toInt());
    QString type = ui->comboBox_TypeSelect->currentText();

    // FOR CHAMBER
    // not emulator!!!
    if(type == "Матрица")
    {
         chamber_ui->connect(ip, outputPort, inputPort);
         for (int i = 0; i < this->connCntMax; ++i)
         {
             std::this_thread::sleep_for(std::chrono::milliseconds(10));
             if(chamber_ui->isConnected())
             {
                 chamber_ui->show();
                 break;
             }

             // last iteration
             if(i == this->connCntMax - 1)
             {
                 //chamber_ui->disconnect();
                 QMessageBox::critical(this, " ", "Ошибка подключения!", QMessageBox::Close);
             }
         }
    }

    // emulator!!!!
    //chamber_ui->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->QMainWindow::closeEvent(event);
}

void MainWindow::on_lineEdit_ip_editingFinished()
{
    QHostAddress ipv4addr ( ui->lineEdit_ip->text() );
    // invalid address check
    if(QAbstractSocket::IPv4Protocol != ipv4addr.protocol())
    {
        ui->lineEdit_ip->setText(default_ip);
    }
}
