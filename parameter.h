#ifndef PARAMETER_H
#define PARAMETER_H

#include "constant.h"

class Parameter : public QWidget
{
    Q_OBJECT

private:
    char *labeltext;
    bool loga;
    int precision_loga;
public:



    Parameter(QWidget *parent = nullptr);
    ~Parameter();
    int value();
    void setText(char *text);
    void setloga(bool isloga,int pre = 1);
    void setRange(int min = 0,int max = 100);
    void setValue(long value);

    QSlider *slider;
    QLabel *label;
signals:
    void will_need_update();
public slots:
    void Update(int _ = 0);
};

#endif // PARAMETER_H
