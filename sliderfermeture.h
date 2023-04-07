#ifndef SLIDERFERMETURE_H
#define SLIDERFERMETURE_H

#include "constant.h"
#include "simulateur.h"



class SliderFermeture : public QWidget
{
    Q_OBJECT
public:
    SliderFermeture(QWidget *parent = nullptr);
    QGroupBox *box;
    QLabel *desc;
    QGridLayout *mainlayout;

    QLabel *global;
    QSlider *Sglobal;

    QLabel *H0;
    QLabel *H7;
    QLabel *H8;
    QLabel *H12;
    QLabel *H13;
    QLabel *H14;
    QLabel *H16;
    QLabel *H18;
    QLabel *H21;
    QSlider *SH0;
    QSlider *SH7;
    QSlider *SH8;
    QSlider *SH12;
    QSlider *SH13;
    QSlider *SH14;
    QSlider *SH16;
    QSlider *SH18;
    QSlider *SH21;

    void setTitle(QString title);
    void setDescription(QString description);
    void apply(state_infr *target);
    void setValues(FILE *file);

signals:
    void will_need_update();

public slots:
    void Update(int _ = 0);

};

#endif // SLIDERFERMETURE_H
