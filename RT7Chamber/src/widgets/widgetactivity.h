#ifndef WIDGETACTIVITY_H
#define WIDGETACTIVITY_H

#include <QWidget>
#include <QFont>
#include "MessageReceiver.h"
#include "MessageTransmitter.h"

namespace Ui {
class WidgetActivity;
}

class WidgetActivity : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetActivity(QWidget *parent = nullptr);
    ~WidgetActivity();
    void registerConnectors(MessageReceiver*, MessageTransmitter*);

    void setMeasuresCompleted(int);

private slots:
    void on_pushButton_startMeasurement_clicked();
    void on_pushButton_resetMeasurement_clicked();

private:
    Ui::WidgetActivity *ui;
    void drawStartStateButton();
    void drawStopStateButton();
    void drawCurrentStartButtonState();
private:
    bool buttonInStartState = 1;
    QFont buttonsFont;
    MessageReceiver* receiver = nullptr;
    MessageTransmitter* transmitter = nullptr;
};

#endif // WIDGETACTIVITY_H
