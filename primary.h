#ifndef PRIMARY_H
#define PRIMARY_H

#include "constant.h"
class primary : public QWidget
{
    Q_OBJECT

    public:

    void WinLayout();

    public:

    QWidget *fenetre;
    QVBoxLayout *VLAy;
    QHBoxLayout *Hbutton;
    QPushButton *button;
    QLabel *version;
    QGroupBox *memory;
    QRadioButton *automatique;
    QRadioButton *ram;
    QRadioButton *ram_multithread;
    QRadioButton *ssd;
    QCheckBox *gpu;
    QVBoxLayout *memory_box;

    preferences *SETTINGS;
};
#endif // PRIMARY_H
