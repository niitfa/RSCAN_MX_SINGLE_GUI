#include "widgetsensivity.h"
#include "ui_widgetsensivity.h"

WidgetSensivity::WidgetSensivity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSensivity)
{
    ui->setupUi(this);

    // internal frame voltage value
    ui->frame_borderInternal->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_borderInternal->setParent(nullptr);
    ui->frame_borderInternal->setLayout(ui->horizontalLayout_borderInternal);

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
    int headIconSize = 20;
    QIcon headIcon(":/img/icon_sensivity.png");
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
    ui->label_headText->setText("Чувствительность");

    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    ui->pushButton_sensivityLow->setFont(buttonsFont);
    ui->pushButton_sensivityLow->setText("Низкая");
    ui->pushButton_sensivityHigh->setFont(buttonsFont);
    ui->pushButton_sensivityHigh->setText("Высокая");

    setSensivity(!this->lastSensivity);
}

WidgetSensivity::~WidgetSensivity()
{
    delete ui;
}

void WidgetSensivity::registerConnectors(MessageReceiver * receiver, MessageTransmitter * transmitter)
{
    this->receiver = receiver;
    this->transmitter = transmitter;
}

void WidgetSensivity::setSensivity(uint8_t sensivity)
{
    if((sensivity != this->lastSensivity))
    {
        if(sensivity)
        {
            this->setHighButtonDisabled();
            this->setLowButtonEnabled();
        }
        else
        {
            this->setLowButtonDisabled();
            this->setHighButtonEnabled();

        }
        this->lastSensivity = sensivity;
    }
}

void WidgetSensivity::setEnabledStyle(QPushButton *button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(220,220,220); }"
                "QPushButton { background-color: rgb(240,240,240); }"
                "QPushButton { color: rgb(20,20,20); }"
                // hover
                "QPushButton:hover { background-color: rgb(230,230,230);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(200,200,200);  }"
                );
    }
}

void WidgetSensivity::setDisabledStyle(QPushButton *button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"
                "QPushButton { color: white; }"
                );
    }
}

void WidgetSensivity::setLowButtonEnabled()
{
    QPushButton* button = ui->pushButton_sensivityLow;
    button->setEnabled(1);
    button->setText("Низкая");
    this->setEnabledStyle(button);
}

void WidgetSensivity::setLowButtonDisabled()
{
    QPushButton* button = ui->pushButton_sensivityLow;
    button->setDisabled(1);
    button->setText("Низкая");
    this->setDisabledStyle(button);
}

void WidgetSensivity::setHighButtonEnabled()
{
    QPushButton* button = ui->pushButton_sensivityHigh;
    button->setEnabled(1);
    button->setText("Высокая");
    this->setEnabledStyle(button);
}

void WidgetSensivity::setHighButtonDisabled()
{
    QPushButton* button = ui->pushButton_sensivityHigh;
    button->setDisabled(1);
    button->setText("Высокая");
    this->setDisabledStyle(button);
}

void WidgetSensivity::on_pushButton_sensivityLow_clicked()
{
    if(transmitter)
    {
        transmitter->setBroadRange();
        //setSensivity(0); // debug!!!!
    }
}

void WidgetSensivity::on_pushButton_sensivityHigh_clicked()
{
    if(transmitter)
    {
        transmitter->setNarrowRange();
        //setSensivity(1); // debug!!!!
    }
}
