#ifndef WIDGETVALUE_H
#define WIDGETVALUE_H

#include <QWidget>
#include <QString>

namespace Ui {
class WidgetValue;
}

class WidgetValue : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetValue(QWidget *parent = nullptr);
    ~WidgetValue();

    void setHeadText(QString qstr);
    void setValueText(QString qstr);

private:
    Ui::WidgetValue *ui;
};

#endif // WIDGETVALUE_H
