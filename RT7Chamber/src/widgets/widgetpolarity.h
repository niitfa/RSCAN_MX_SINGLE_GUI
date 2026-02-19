#ifndef WIDGETPOLARITY_H
#define WIDGETPOLARITY_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include "MessageReceiver.h"
#include "MessageTransmitter.h"

namespace Ui {
class WidgetPolarity;
}

class WidgetPolarity : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPolarity(QWidget *parent = nullptr);
    ~WidgetPolarity();
    void registerConnectors(MessageReceiver*, MessageTransmitter*);
    void setPolarity(uint8_t);
    void enableWidget(bool);
private slots:
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();

private:
    void setEnabledStyle(QPushButton*);
    void setDisabledStyle(QPushButton*);
    void setInactiveStyle(QPushButton*);

    void setPlusButtonEnabled();
    void setPlusButtonDisabled();
    void setMinusButtonEnabled();
    void setMinusButtonDisabled();
private:
    Ui::WidgetPolarity *ui;
    QFont buttonsFont;
    uint8_t lastPolarity = 1;
    uint8_t lastEnabled = 0;
    MessageReceiver* receiver = nullptr;
    MessageTransmitter* transmitter = nullptr;
};

#endif // WIDGETPOLARITY_H
