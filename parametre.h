#ifndef PARAMETRE_H
#define PARAMETRE_H

#include "simulateur.h"
#include "sliderfermeture.h"
#include "parameter.h"


class ParaMesure : public QWidget
{
    Q_OBJECT
public:
    ParaMesure();
    ~ParaMesure();
    void setup(Simulateur *sim = NULL);
    void setsimulateur(Simulateur *sim,compilateur *compil);
    void reset(const QString path);
    Simulateur *simul;
    compilateur *compil;

    Parameter *teletravail;
    Parameter *distanciation;
    Parameter *nbmasque;
    Parameter *effmasque;
    Parameter *vaccinationjournaliere;
    Parameter *autoconfinement;
    Parameter *hopital_re;
    SliderFermeture *ecole;
    SliderFermeture *commerce;
    SliderFermeture *industriel;
    SliderFermeture *loisir;

    QScrollArea *fenetre;
    QWidget *container;

    QVBoxLayout *layoutcontainer;
    QGridLayout *layoutparameter;
    QVBoxLayout *layoutfermeture;
public slots:
    void Update_passif(int _ = 0);

};

class ParaVirus : public QWidget
{
    Q_OBJECT
public:
    ParaVirus();
    ~ParaVirus();
    void setup(Simulateur *sim = NULL);
    void setsimulateur(Simulateur *sim,compilateur *compil);
    void Update_actif(int _ = 0);
    void reset(const QString path);
    Simulateur *simul;
    compilateur *compil;


    Parameter *efficacite_zone;
    Parameter *distance_zone;
    Parameter *efficacite_famille;
    Parameter *facteur_degat;
    Parameter *temps_infection;
    Parameter *debut_infecteux;
    Parameter *temps_infecteux;
    Parameter *nouveau;
    Parameter *effvaccin;
    Parameter *factor_distance;
    QWidget *fenetre;
    QVBoxLayout *VLayout;

signals:
    void Update_need();

public slots:
    void Update_passif(int _ = 0);
    void Soon_Update(int _ = 0);
};


#endif // PARAMETRE_H
