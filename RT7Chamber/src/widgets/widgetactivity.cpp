#include "widgetactivity.h"
#include "ui_widgetactivity.h"
#include <QIcon>
#include <QFont>

WidgetActivity::WidgetActivity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetActivity)
{
    ui->setupUi(this);

    // internal frame
    ui->frame_internalBorder->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_internal->setParent(nullptr); //horizontalLayout_internal
    ui->frame_internalBorder->setLayout(ui->horizontalLayout_internal);

    // external frame
    ui->frame_externalBorder->setStyleSheet(
                " QFrame { background-color: rgb(230,230,230); } "
                " QFrame { border-radius:15px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );
    ui->verticalLayout_external->setParent(nullptr);
    ui->frame_externalBorder->setLayout(ui->verticalLayout_external);

    // icon
    QIcon headIcon = QIcon(":/img/icon_activity.png");
    int iconSize = 24;
    ui->label_headIcon->clear();
    ui->label_headIcon->setPixmap(headIcon.pixmap(iconSize, iconSize));
    ui->label_headIcon->setStyleSheet(
                "border-width:0px;"
                );

    // head text
    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(50);

    QString headTextColor = "color: black;";
    ui->label_head->clear();
    ui->label_head->setStyleSheet(
                "border-width:0px;" +
                headTextColor
                );
    ui->label_head->setFont(headFont);
    ui->label_head->setText("Активность");

    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    // start/stop button
    this->drawCurrentStartButtonState();
    ui->pushButton_startMeasurement->setStyleSheet(
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
    ui->pushButton_startMeasurement->setFocusPolicy( Qt::FocusPolicy::NoFocus );

    // reset button
    int resetButtonIconSize = 24;
    QIcon resetButtonIcon = QIcon(":/img/button_icon_reset.png");
    ui->pushButton_resetMeasurement->setIcon(resetButtonIcon);
    ui->pushButton_resetMeasurement->setIconSize(QSize(resetButtonIconSize, resetButtonIconSize));
    ui->pushButton_resetMeasurement->setFont(buttonsFont);
    ui->pushButton_resetMeasurement->setText("Сбросить");
    ui->pushButton_resetMeasurement->setFocusPolicy( Qt::FocusPolicy::NoFocus );

    // label completed measurements
    ui->label_measuresCompleted->setStyleSheet(
                "border-width:0px;"
                );
    ui->label_measuresCompleted->setFont(buttonsFont);
    ui->label_measuresCompleted->setText("Выполнено:");

    // label measure text
    ui->label_measureTask->setStyleSheet(
                "border-width:0px;"
                );
    ui->label_measureTask->setFont(buttonsFont);
    ui->label_measureTask->setText("Кол-во измерений:");

    // label measure num
    ui->label_measuresNum->setStyleSheet(
                "border-width:0px;"
                );
    ui->label_measuresNum->setFont(buttonsFont);
    ui->label_measuresNum->setText(QString::number(0));

    // line edit measure task
    ui->lineEdit_measuresTask->setStyleSheet(
                "border-width:0px;"
                );
    ui->lineEdit_measuresTask->setFont(buttonsFont);
    ui->lineEdit_measuresTask->setText(QString::number(100));
}

WidgetActivity::~WidgetActivity()
{
    delete ui;
}

void WidgetActivity::setMeasuresCompleted(int val)
{
    ui->label_measuresNum->setText(QString::number(val));
}

void WidgetActivity::registerConnectors(MessageReceiver *receiver, MessageTransmitter *transmitter)
{
    this->receiver = receiver;
    this->transmitter = transmitter;
}

void WidgetActivity::drawStartStateButton()
{
    int startButtonIconSize = 16;
    QIcon startButtonIcon = QIcon(":/img/button_icon_start.png");
    ui->pushButton_startMeasurement->setIcon(startButtonIcon);
    ui->pushButton_startMeasurement->setIconSize(QSize(startButtonIconSize,startButtonIconSize));
    ui->pushButton_startMeasurement->setFont(buttonsFont);
    ui->pushButton_startMeasurement->setText("Начать измерение");

}

void WidgetActivity::drawStopStateButton()
{
    int stopButtonIconSize = 24;
    QIcon stopButtonIcon = QIcon(":/img/button_icon_pause.png");
    ui->pushButton_startMeasurement->setIcon(stopButtonIcon);
    ui->pushButton_startMeasurement->setIconSize(QSize(stopButtonIconSize,stopButtonIconSize));
    ui->pushButton_startMeasurement->setFont(buttonsFont);
    ui->pushButton_startMeasurement->setText("Остановить измерение");
}

void WidgetActivity::drawCurrentStartButtonState()
{
    if(this->buttonInStartState)
    {
        this->drawStartStateButton();
    }
    else
    {
        this->drawStopStateButton();
    }
}


void WidgetActivity::on_pushButton_startMeasurement_clicked()
{
    // start meas
    if(this->transmitter)
    {
        int cycles = ui->lineEdit_measuresTask->text().toInt();
        transmitter->startMeasurement(cycles);
    }
}

void WidgetActivity::on_pushButton_resetMeasurement_clicked()
{
    if(this->transmitter)
    {
        transmitter->resetMeasurement();
    }
}
