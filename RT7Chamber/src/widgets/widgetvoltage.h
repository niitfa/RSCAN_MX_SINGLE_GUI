#ifndef WIDGETVOLTAGE_H
#define WIDGETVOLTAGE_H

#include <QWidget>
#include <QFont>
#include "MessageReceiver.h"
#include "MessageTransmitter.h"


namespace Ui {
class WidgetVoltage;
}

class WidgetVoltage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVoltage(QWidget *parent = nullptr);
    ~WidgetVoltage();
    void registerConnectors(MessageReceiver*, MessageTransmitter*);
    int getInputVoltage();
private slots:
    void on_pushButton_changeVoltage_clicked();

private:
    Ui::WidgetVoltage *ui;
    QFont buttonsFont;
    MessageReceiver* receiver = nullptr;
    MessageTransmitter* transmitter = nullptr;
};

#endif // WIDGETVOLTAGE_H
