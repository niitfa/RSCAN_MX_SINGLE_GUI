#ifndef WIDGETSENSIVITY_H
#define WIDGETSENSIVITY_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include "MessageReceiver.h"
#include "MessageTransmitter.h"

namespace Ui {
class WidgetSensivity;
}

class WidgetSensivity : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSensivity(QWidget *parent = nullptr);
    ~WidgetSensivity();
    void registerConnectors(MessageReceiver*, MessageTransmitter*);
    void setSensivity(uint8_t);
private slots:
    void on_pushButton_sensivityLow_clicked();

    void on_pushButton_sensivityHigh_clicked();

private:
    void setEnabledStyle(QPushButton*);
    void setDisabledStyle(QPushButton*);

    void setLowButtonEnabled();
    void setLowButtonDisabled();
    void setHighButtonEnabled();
    void setHighButtonDisabled();
private:
    Ui::WidgetSensivity *ui;
    QFont buttonsFont;
    uint8_t lastSensivity = 1; // 0 - low, 1 - high
    MessageReceiver* receiver = nullptr;
    MessageTransmitter* transmitter = nullptr;
};

#endif // WIDGETSENSIVITY_H
