#include "primary.h"

void primary::WinLayout()
{
    fenetre = new QWidget();
    fenetre->resize(400,220);
    fenetre->setMinimumSize(400,220);
    VLAy = new QVBoxLayout(fenetre);
    button = new QPushButton("Start");
    button->setFixedSize(80,23);
    version = new QLabel("TIPE v alpha par Noé Loisil");
    memory = new QGroupBox();
    memory_box = new QVBoxLayout(memory);

    automatique = new QRadioButton();
    ram = new QRadioButton();
    ram_multithread = new QRadioButton();
    ssd = new QRadioButton();
    gpu = new QCheckBox();
    gpu->setText("Utiliser le gpu");
    memory->setTitle("paramètre mémoire et optimisations :");
    automatique->setText("auto");
    ram->setText("ram");
    ram_multithread->setText("ram + multi-thread ( !seed )");
    ssd->setText("ssd");
    memory_box->addWidget(automatique);
    memory_box->addWidget(ram);
    memory_box->addWidget(ram_multithread);
    memory_box->addWidget(ssd);
    memory_box->addWidget(gpu);
    Hbutton = new QHBoxLayout();
    Hbutton->addWidget(button);
    memory->setMinimumSize(250,150);
    VLAy->addLayout(Hbutton);
    VLAy->addWidget(memory);
    VLAy->addWidget(version);
    if(SETTINGS->automatique)
        automatique->setChecked(true);
    else if(SETTINGS->ram)
        if(SETTINGS->multi_thread)
            ram_multithread->setChecked(true);
        else
            ram->setChecked(true);
    else
        ssd->setChecked(true);
    if(SETTINGS->gpu)
        gpu->setChecked(true);
    fenetre->show();
}
