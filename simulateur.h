#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include "compilateur.h"
#include "loading_bar.h"

class humain
{
public:
    float age;
    bool dead;
    int maladie;
    float pv;
    Sint16 hab_x;
    Sint16 hab_y;
    Sint16 job_x;
    Sint16 job_y;
    bool hopital;

    Uint8 gravite;

    long *famille;
    Uint8 nbfamille;


    void operator=(humain *target)
    {
         std::copy(target,target+sizeof(humain),this);
    }
};
#define SSD_HUMAN_SIZE 10
class humanite
{
public:
    humain **population;
    Uint64 nb_partition;

    Uint64 size_partition;
    Uint64 size_last_partition;
    Uint64 nb;
    Uint8 logpartition;
};


class data
{
public:

    QVector<double> all;
    bool increment;
    QVector<double> *all_incr;

    unsigned long date;
    data(bool hasincr = false,unsigned int factor = 1);
    void reset(long value = 0);
    void add(long *data,long size);
    void add(long data);
    void add(double date);
    QVector<double> timeline(bool incr = false);
};

class state_infr
{
public:
    pourcentage ouverture[9];
};

class counter
{
public:
    Uint64 *data;
    void reset();
    void add(int x,int y,int factor,int distance_zone,Uint64 *distanciel);
    void alloc(int w,int h);
    Uint64 get(int x,int y);
    int w,h;
};


class Simulateur
{
public:
    Simulateur(FILE *file,compilateur *c,loading_bar *load,QString dossier,long seed,char *path_map);
    ~Simulateur();

    char* getDate();

    void tranches(unsigned long n);
    void tranche();
    long getnbendessous(int age);
    long getnbaudessus(int age);
    long getnb(int age);

    long memory_taked();
    long getnbdeath();
    long getnbsains();
    long getnbgueris();
    long getnbinfecte();
    double getmoyenneagemort();
    double getmoyenneageinfecte();
    double getmoyenneage();
    double degat(int gravite,int tranche);
    void addcontamine(humain *cible,int factor);

    QVector<double> timeline();


    long nb;
    humain *population;
    Uint8 tranche_actuel;
    int jour;
    compilateur *compil;
    QImage *im;
    long height,width,imsize;
    long nbenfant;

    double precalcdegat[48384];
    data hopital;
    data nb_mort;
    data nb_sains;
    data nb_infecte;
    data nb_gueris;
    data dmort;
    data dinfecte;
    data dgueris;
    data moyenne_age_infecte;
    data moyenne_age_mort;

    QVector<double> moyenne;
    double moyenne_age;

    counter counter_habitation;
    counter counter_industriel;
    counter counter_commerce;
    counter counter_loisir;
    counter counter_school;
    counter counter_densite;
    double *inverse_densite;

    QVector<double> all;
    QVector<double> all_jour;
    loading_bar *loading;


    // paramètres
    pourcentage teletravail;
    pourcentage distanciation;
    pourcentage nbmasque;
    pourcentage effmasque;
    long vaccinationjournaliere;
    long nouveau;
    pourcentage autoconfinement;
    pourcentage notautoconfinement;
    long hopital_re;
    long act_hop;
    state_infr ecole;
    state_infr commerce;
    state_infr industriel;
    state_infr loisir;

    pourcentage efficacite_zone;
    int distance_zone;
    pourcentage efficacite_famille;
    double facteur_degat;
    int debut_infecteux;
    int temps_infecteux;
    int temps_infection;
    pourcentage effvaccin;
    pourcentage factor_distance;
    Uint64 *distanciel;



    long mal_prop;

private:
    long adopte();
    void setup_age(FILE* file);
    void setup_family(FILE* file);
    void setup_geo(FILE* file);
    void count(int factor,Uint16 x,Uint16 y);







};

#endif // SIMULATEUR_H
