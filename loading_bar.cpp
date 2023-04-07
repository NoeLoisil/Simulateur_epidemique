#include "loading_bar.h"
#include <QtGui>


loading_bar::loading_bar(QWidget *parent)
{
    this->setFixedSize(600,120);
    box = new QGridLayout(this);
    bar = new QProgressBar();
    frac = new QLabel();
    etat = new QLabel();
    //time = new QLabel();


    box->addWidget(bar,0,0,1,2);
    box->addWidget(frac,1,0);
    //box->addWidget(time,1,1);
    box->addWidget(etat,2,0,1,2);
}

QString loading_bar_tampon;
void loading_bar::reset(Uint64 size,QString state)
{
    final = size;
    //moy = 0;
    //last_actualise = clock();
    bar->setValue(0);

    frac->setText(loading_bar_tampon.asprintf("0/%llu",final));
    //time->setText("");
    if(state == NULL)
        etat->setText("");
    else
        etat->setText(state);
   this->show();
   qApp->processEvents();
}

void loading_bar::actualise(Uint64 complet,QString state)
{
    if(!this->isHidden())
    {
        //last_actualise = clock();
        bar->setValue(100*complet/final);
        frac->setText(loading_bar_tampon.asprintf("%llu/%llu",complet,final));
        //time->setText("Temps restant : 00h00m00s");
        if(state != NULL)
            etat->setText(state);
        qApp->processEvents();
    }
}

void loading_bar::end()
{
    this->hide();
}
