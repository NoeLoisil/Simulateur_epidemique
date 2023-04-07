#include "sliderfermeture.h"

QFont *smallfont = new QFont("Times",10);
char *sblanket = challoc(1024);
SliderFermeture::SliderFermeture(QWidget *parent)
{
    box = new QGroupBox(this);
    box->setTitle("title");
    mainlayout = new QGridLayout(box);
    desc = new QLabel();
    desc->setText("description");

    global = new QLabel();
    global->setText("global  ( 100% )");
    global->setFont(*smallfont);
    Sglobal = new QSlider(Qt::Orientation::Horizontal);
    Sglobal->setRange(0,100);
    Sglobal->setSliderPosition(100);
    H0 = new QLabel();
    H0->setFont(*smallfont);
    H0->setText("00h-07h ( 100% )");
    H7 = new QLabel();
    H7->setFont(*smallfont);
    H7->setText("07h-08h ( 100% )");
    H8 = new QLabel();
    H8->setFont(*smallfont);
    H8->setText("08h-12h ( 100% )");
    H12 = new QLabel();
    H12->setFont(*smallfont);
    H12->setText("12h-13h ( 100% )");
    H13 = new QLabel();
    H13->setFont(*smallfont);
    H13->setText("13h-14h ( 100% )");
    H14 = new QLabel();
    H14->setFont(*smallfont);
    H14->setText("14h-16h ( 100% )");
    H16 = new QLabel();
    H16->setFont(*smallfont);
    H16->setText("16h-18h ( 100% )");
    H18 = new QLabel();
    H18->setFont(*smallfont);
    H18->setText("18h-21h ( 100% )");
    H21 = new QLabel();
    H21->setFont(*smallfont);
    H21->setText("21h-00h ( 100% )");
    SH0 = new QSlider(Qt::Orientation::Horizontal);
    SH7 = new QSlider(Qt::Orientation::Horizontal);
    SH8 = new QSlider(Qt::Orientation::Horizontal);
    SH12 = new QSlider(Qt::Orientation::Horizontal);
    SH13 = new QSlider(Qt::Orientation::Horizontal);
    SH14 = new QSlider(Qt::Orientation::Horizontal);
    SH16 = new QSlider(Qt::Orientation::Horizontal);
    SH18 = new QSlider(Qt::Orientation::Horizontal);
    SH21 = new QSlider(Qt::Orientation::Horizontal);
    SH0->setRange(0,100);
    SH7->setRange(0,100);
    SH8->setRange(0,100);
    SH12->setRange(0,100);
    SH13->setRange(0,100);
    SH14->setRange(0,100);
    SH16->setRange(0,100);
    SH18->setRange(0,100);
    SH21->setRange(0,100);
    SH0->setSliderPosition(100);
    SH7->setSliderPosition(100);
    SH8->setSliderPosition(100);
    SH12->setSliderPosition(100);
    SH13->setSliderPosition(100);
    SH14->setSliderPosition(100);
    SH16->setSliderPosition(100);
    SH18->setSliderPosition(100);
    SH21->setSliderPosition(100);
    mainlayout->addWidget(desc,0,0,1,4);
    mainlayout->addWidget(global,1,0);
    mainlayout->addWidget(H0,1,1);
    mainlayout->addWidget(H7,1,2);
    mainlayout->addWidget(H8,1,3);
    mainlayout->addWidget(H12,1,4);
    mainlayout->addWidget(H13,3,0);
    mainlayout->addWidget(H14,3,1);
    mainlayout->addWidget(H16,3,2);
    mainlayout->addWidget(H18,3,3);
    mainlayout->addWidget(H21,3,4);
    mainlayout->addWidget(Sglobal,2,0);
    mainlayout->addWidget(SH0,2,1);
    mainlayout->addWidget(SH7,2,2);
    mainlayout->addWidget(SH8,2,3);
    mainlayout->addWidget(SH12,2,4);
    mainlayout->addWidget(SH13,4,0);
    mainlayout->addWidget(SH14,4,1);
    mainlayout->addWidget(SH16,4,2);
    mainlayout->addWidget(SH18,4,3);
    mainlayout->addWidget(SH21,4,4);
    connect(Sglobal,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH0,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH7,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH8,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH12,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH13,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH14,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH16,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH18,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
    connect(SH21,SIGNAL(valueChanged(int)),this,SLOT(Update(int)));
}

bool isfree = true;

void SliderFermeture::Update(int _)
{
    if(isfree)
    {
        isfree = false;
        sprintf(sblanket,"global  ( %3d%% )",Sglobal->sliderPosition());
        global->setText(sblanket);
        sprintf(sblanket,"00h-07h ( %3d%% )",SH0->sliderPosition());
        H0->setText(sblanket);H0->adjustSize();
        sprintf(sblanket,"07h-08h ( %3d%% )",SH7->sliderPosition());
        H7->setText(sblanket);H7->adjustSize();
        sprintf(sblanket,"08h-12h ( %3d%% )",SH8->sliderPosition());
        H8->setText(sblanket);H8->adjustSize();
        sprintf(sblanket,"12h-13h ( %3d%% )",SH12->sliderPosition());
        H12->setText(sblanket);H12->adjustSize();
        sprintf(sblanket,"13h-14h ( %3d%% )",SH13->sliderPosition());
        H13->setText(sblanket);H13->adjustSize();
        sprintf(sblanket,"14h-16h ( %3d%% )",SH14->sliderPosition());
        H14->setText(sblanket);H14->adjustSize();
        sprintf(sblanket,"16h-18h ( %3d%% )",SH16->sliderPosition());
        H16->setText(sblanket);H16->adjustSize();
        sprintf(sblanket,"18h-21h ( %3d%% )",SH18->sliderPosition());
        H18->setText(sblanket);H18->adjustSize();
        sprintf(sblanket,"21h-00h ( %3d%% )",SH21->sliderPosition());
        H21->setText(sblanket);H21->adjustSize();
        emit will_need_update();
        isfree = true;
    }
}
void SliderFermeture::setTitle(QString title)
{
    box->setTitle(title);
    box->adjustSize();
}

void SliderFermeture::setDescription(QString description)
{
    desc->setText(description);
    desc->adjustSize();
}

void SliderFermeture::apply(state_infr *target)
{
    target->ouverture[0] = SH0->value()*Sglobal->value();
    target->ouverture[1] = SH7->value()*Sglobal->value();
    target->ouverture[2] = SH8->value()*Sglobal->value();
    target->ouverture[3] = SH12->value()*Sglobal->value();
    target->ouverture[4] = SH13->value()*Sglobal->value();
    target->ouverture[5] = SH14->value()*Sglobal->value();
    target->ouverture[6] = SH16->value()*Sglobal->value();
    target->ouverture[7] = SH18->value()*Sglobal->value();
    target->ouverture[8] = SH21->value()*Sglobal->value();
}

void SliderFermeture::setValues(FILE *file)
{
    char cc = fgetc(file);
    while(cc != '{')
        cc = fgetc(file);
    long V0,V7,V8,V12,V13,V14,V16,V18,V21;
    fscanf(file,"%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld}\n",&V0,&V7,&V8,&V12,&V13,&V14,&V16,&V18,&V21);
    Sglobal->setValue(100);
    SH0->setValue(V0);SH7->setValue(V7);SH8->setValue(V8);
    SH12->setValue(V12);SH13->setValue(V13);SH14->setValue(V14);
    SH16->setValue(V16);SH18->setValue(V18);SH21->setValue(V21);
    Update();
}
