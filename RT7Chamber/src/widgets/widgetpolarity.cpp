#include "widgetpolarity.h"
#include "ui_widgetpolarity.h"

WidgetPolarity::WidgetPolarity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPolarity)
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
    QIcon headIcon(":/img/icon_polarity.png");
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
    ui->label_headText->setText("Полярность");


    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(30);
    buttonsFont.setWeight(50);

    // change plus-minus buttons
    //enableWidget(!this->lastEnabled);
    setPolarity(!this->lastPolarity);
}

WidgetPolarity::~WidgetPolarity()
{
    delete ui;
}

void WidgetPolarity::registerConnectors(MessageReceiver* receiver, MessageTransmitter* transmitter)
{
    this->receiver = receiver;
    this->transmitter = transmitter;
}

void WidgetPolarity::setEnabledStyle(QPushButton * button)
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
                // hover
                "QPushButton:hover { background-color: rgb(230,230,230);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(200,200,200);  }"
                );
    }
}

void WidgetPolarity::setDisabledStyle(QPushButton * button)
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
                );
    }
}

void WidgetPolarity::setInactiveStyle(QPushButton* button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:0px; }"
                "QPushButton { background-color: rgb(129,142,172); }"
                );
    }
}

void WidgetPolarity::setPlusButtonEnabled()
{
    QPushButton* button = ui->pushButton_plus;
    button->setEnabled(1);
    QIcon icon (":/img/icon_plus_dark.png");
    int iconSize = 22;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setEnabledStyle(button);
}

void WidgetPolarity::setPlusButtonDisabled()
{
    QPushButton* button = ui->pushButton_plus;
    button->setDisabled(1);
    QIcon icon (":/img/icon_plus_bright.png");
    int iconSize = 22;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setDisabledStyle(button);
}

void WidgetPolarity::setMinusButtonEnabled()
{
    QPushButton* button = ui->pushButton_minus;
    button->setEnabled(1);
    QIcon icon (":/img/icon_minus_dark.png");
    int iconSize = 25;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setEnabledStyle(button);
}

void WidgetPolarity::setMinusButtonDisabled()
{
    QPushButton* button = ui->pushButton_minus;
    button->setDisabled(1);
    QIcon icon (":/img/icon_minus_bright.png");
    int iconSize = 25;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setDisabledStyle(button);
}

void WidgetPolarity::on_pushButton_plus_clicked()
{
    if(transmitter)
    {
        transmitter->setPositiveVoltage();
        //setPolarity(0); // debug!!!!
    }
}

void WidgetPolarity::on_pushButton_minus_clicked()
{
    if(transmitter)
    {
        transmitter->setNegativeVoltage();
        //setPolarity(1); // debug!!!!
    }
}

void WidgetPolarity::setPolarity(uint8_t polarity)
{
    if((polarity != this->lastPolarity))
    {
        if(polarity) // negative
        {
            this->setMinusButtonDisabled();
            this->setPlusButtonEnabled();
        }
        else // positive
        {
            this->setPlusButtonDisabled();
            this->setMinusButtonEnabled();

        }
        this->lastPolarity = polarity;
    }
}

void WidgetPolarity::enableWidget(bool enabled)
{
    if(enabled != this->lastEnabled)
    {
        if(enabled)
        {
            this->lastPolarity = !this->lastPolarity;
            this->setPolarity(!this->lastPolarity);
        }
        else
        {
            ui->pushButton_plus->setDisabled(1);
            ui->pushButton_minus->setDisabled(1);
            if(this->lastPolarity)
            {

                this->setInactiveStyle(ui->pushButton_minus);
            }
            else
            {
                // shadow positive
                this->setInactiveStyle(ui->pushButton_plus);
            }
        }
        this->lastEnabled = enabled;
    }
}

