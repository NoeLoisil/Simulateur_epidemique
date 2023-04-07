#ifndef PERSO_H
#define PERSO_H

#include "fenetre.h"
#include "compilateur.h"
#include "simulateur.h"

class Perso : public QWidget
{
    Q_OBJECT
public:

    bool retracted;

    Fenetre *fenetre;
    QPushButton *go;
    QSpinBox *id_choose;
    QPushButton *lucky;
    QGroupBox *infos;
    QLabel *id;
    QLabel *nom;
    QLabel *age;
    QLabel *maladie;
    QLabel *chance;

    QProgressBar *bar;

    QImage *map;
    QLabel *supmap;
    int factor;


    QPushButton *tuer;
    QPushButton *infecter;
    QPushButton *modifier;

    QGroupBox *famille;
    QLabel *id_famille[10];

    void *next;
    void *previous;
    int index;
    void **origin;
    compilateur *compil;
    Simulateur *simul;

    long actual_id;
    Perso();
    ~Perso();
    void layout(int position,compilateur *comp,Simulateur *sim);


public slots:

    void destroy();
    void destroy_all();
    void change_id(int _ = 0);
    void change_id_lucky();
    void kill();
    void infecte();
signals:
    void actualise(int position);
    void actualise_target(int position);
};

#endif
