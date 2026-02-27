#include "widgetfile.h"
#include "ui_widgetfile.h"

// todo
// 1. session.update();

WidgetFile::WidgetFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetFile)
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
    int headIconSize = 24;
    QIcon headIcon(":/img/icon_file.png");
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
    ui->label_headText->setText("Сохранение сессии");

    // buttons
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    // label poriod
    ui->label_period->setStyleSheet(
                "border-width:0px;" +
                headTextColor
                );
    ui->label_period->setFont(buttonsFont);
    ui->label_period->setText("Период:");

    // line edit
    ui->lineEdit_periodInput->setFont(buttonsFont);
    ui->lineEdit_periodInput->setText(QString::number(this->fileUpdatePeriod));

    // button
    this->setStartStyle(ui->pushButton_start);
}

WidgetFile::~WidgetFile()
{
    delete ui;
}

void WidgetFile::setSession(ScanSessionFile *session)
{
    this->session = session;
}

void WidgetFile::on_lineEdit_periodInput_editingFinished()
{
    int value = ui->lineEdit_periodInput->text().toInt();
    if (value <= 0)
    {
        this->fileUpdatePeriod = 1;
        ui->lineEdit_periodInput->setText(QString::number(this->fileUpdatePeriod));
    }
    else
    {
        this->fileUpdatePeriod = value;
    }
}

void WidgetFile::setStartStyle(QPushButton *button)
{
    QIcon icon(":/img/button_icon_start.png");
    int iconSize = 16;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText(" Запись");

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

void WidgetFile::setStopStyle(QPushButton *button)
{
    QIcon icon(":/img/button_icon_stop.png");
    int iconSize = 16;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText(" Стоп");

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

void WidgetFile::on_pushButton_start_clicked()
{
    if(this->session)
    {
        if(this->started)
        {
            ui->lineEdit_periodInput->setEnabled(1);
            setStartStyle(ui->pushButton_start);
            session->stop();
            this->started = 0;
        }
        else
        {
            ui->lineEdit_periodInput->setDisabled(1);
            setStopStyle(ui->pushButton_start);
            if(session->start())
            {
                this->pointIndex = 0; // reset points counter
                this->started = 1;
            }
        }
    }
}

void WidgetFile::update(int id, int value)
{
    if(this->pointIndex < (id / this->fileUpdatePeriod))
    {
        this->pointIndex = id / this->fileUpdatePeriod;
        if(this->session)
        {
            //this->session->update({id, value});
        }
    }
}
