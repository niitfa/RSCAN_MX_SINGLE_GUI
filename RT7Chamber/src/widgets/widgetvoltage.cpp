#include "widgetvoltage.h"
#include "ui_widgetvoltage.h"
#include <QIcon>
#include <QMessageBox>

WidgetVoltage::WidgetVoltage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetVoltage)
{
    ui->setupUi(this);

    // internal frame voltage value
    ui->frame_borderValue->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_borderValue->setParent(nullptr);
    ui->frame_borderValue->setLayout(ui->horizontalLayout_borderValue);

    // external frame
    ui->frame_borderExternal->setStyleSheet(
                " QFrame { background-color: rgb(230,230,230); } "
                " QFrame { border-radius:15px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->verticalLayout_borderExternal->setParent(nullptr);
    ui->frame_borderExternal->setLayout(ui->verticalLayout_borderExternal);

    // icon
    int headIconSize = 24;
    QIcon headIcon(":/img/icon_voltage.png");
    ui->label_headIcon->clear();
    ui->label_headIcon->setPixmap(headIcon.pixmap(headIconSize,headIconSize));
    ui->label_headIcon->setStyleSheet(
                "border-width:0px;"
                );

    // head text
    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(50);

    QString headTextColor = "color: black;";
    ui->label_headText->clear();
    ui->label_headText->setStyleSheet(
                "border-width:0px;" +
                headTextColor
                );
    ui->label_headText->setFont(headFont);
    ui->label_headText->setText("Напряжение");


    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    // change voltage button
    ui->pushButton_changeVoltage->setFont(buttonsFont);
    ui->pushButton_changeVoltage->setText("Изменить напряжение");
    ui->pushButton_changeVoltage->setFocusPolicy( Qt::FocusPolicy::NoFocus );
    ui->pushButton_changeVoltage->setStyleSheet(
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

    // label Voltage value measurements
    ui->label_value->setStyleSheet(
                "border-width:0px;"
                );
    ui->label_value->setFont(buttonsFont);
    //ui->label_value->setText("Величина, В:");
    ui->label_value->setText("Величина, В:");

    // line edit voltage input
    ui->lineEdit_voltageInput->setStyleSheet(
                "border-width:0px;"
                );
    ui->lineEdit_voltageInput->setFont(buttonsFont);
    ui->lineEdit_voltageInput->setText(QString::number(0));
}

WidgetVoltage::~WidgetVoltage()
{
    delete ui;
}

void WidgetVoltage::registerConnectors(MessageReceiver *receiver, MessageTransmitter *transmitter)
{
    this->receiver = receiver;
    this->transmitter = transmitter;
}

int WidgetVoltage::getInputVoltage()
{
    return ui->lineEdit_voltageInput->text().toInt();
}

void WidgetVoltage::on_pushButton_changeVoltage_clicked()
{
    int maxVoltage = 500;
    int targetVoltage = ui->lineEdit_voltageInput->text().toInt();
    if(this->transmitter)
    {
        if(targetVoltage < 0 || targetVoltage > maxVoltage) // check range
        {
            // bad value
            QMessageBox::critical(this,
                                  "Ошибка!",
                                  "Допустимый диапазон:\nV мин. = 0 В\nV макс. = " + QString::number(maxVoltage) + " B"
                                  );
        }
        else
        {
            // good value
            this->transmitter->setVoltageValue(static_cast<uint16_t>(targetVoltage));
        }
    }
}
