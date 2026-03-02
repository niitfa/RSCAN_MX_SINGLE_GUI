#ifndef WIDGETVALUE_H
#define WIDGETVALUE_H

#include <QWidget>
#include <QString>
#include "qgraph.h"

namespace Ui {
class WidgetValue;
}

class WidgetValue : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetValue(QWidget *parent = nullptr);
    ~WidgetValue();

    void setValueText(QString qstr);
    void setGraph(QGraph* qgraph, int index);
    void setChecked(bool);

private slots:
    void on_checkBox_clicked();

private:
    Ui::WidgetValue *ui;

    QGraph* qgraph = nullptr;
    int graphIndex = 0;
    static const int lineLength = 4;

private:
    void setHeadText(QString qstr);
};

#endif // WIDGETVALUE_H
