#include "widgetvalue.h"
#include "ui_widgetvalue.h"

WidgetValue::WidgetValue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetValue)
{
    ui->setupUi(this);

    setValueText("0");
    setHeadText("Description, unit");

    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(55);

    QFont valueFont;
    valueFont.setFamily("Inter");
    valueFont.setPixelSize(40); // 28
    valueFont.setWeight(60);

    // border
    ui->frame_border->setStyleSheet(
                " QFrame { background-color: white; } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(180,180,180); }"
                );

    ui->verticalLayout_border->setParent(nullptr);
    ui->frame_border->setLayout(ui->verticalLayout_border);

    QString colorText = "color: rgb(50,70,100);";
    // head text
    ui->label_head->setFont(headFont);
    ui->label_head->setStyleSheet(
                "border-width:0px;" +
                colorText
                );

    // value text
    ui->label_value->setFont(valueFont);
    ui->label_value->setStyleSheet(
                "border-width:0px;" +
                colorText
                );
}

WidgetValue::~WidgetValue()
{
    delete ui;
}

void WidgetValue::setHeadText(QString qstr)
{
    ui->label_head->setText(qstr);
}

void WidgetValue::setValueText(QString qstr)
{
    ui->label_value->setText(qstr);
}
