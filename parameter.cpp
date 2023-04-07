#include "parameter.h"

Parameter::Parameter(QWidget *parent)
{
    this->setFixedSize(250,40);
    label = new QLabel(this);
    slider = new QSlider(Qt::Orientation::Horizontal,this);
    slider->setGeometry(0,20,200,10);
    setRange();
    loga = false;
    labeltext = "";
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
}

Parameter::~Parameter()
{
    delete label;
    delete slider;
    free(labeltext);
}

void Parameter::setText(char *text)
{
    labeltext = text;
    Update();
}

double log_of_10 = log(10);

int Parameter::value()
{
    if(loga)
    {
        if(slider->sliderPosition() == -1)
            return 0;
        else
            return int(exp(log_of_10*slider->sliderPosition()/precision_loga)+0.5);
    }

    return slider->sliderPosition();
}

void Parameter::setloga(bool isloga,int pre)
{
    slider->setValue(0);
    precision_loga = pre;
    loga = isloga;
}

void Parameter::setRange(int min,int max)
{
    slider->setMinimum(min);
    slider->setMaximum(max);
}

void Parameter::Update(int _)
{
    emit will_need_update();
    char* pblanket = challoc(256);
    sprintf(pblanket,labeltext,value());
    label->setText(pblanket);
    free(pblanket);
    label->adjustSize();
}

void Parameter::setValue(long value)
{
    if(loga)
        slider->setValue(log10(value)*precision_loga+0.5);
    else
        slider->setValue(value);
}
