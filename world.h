#ifndef WORLD_H
#define WORLD_H

#include "compilateur.h"
#include "simulateur.h"
#include "map.h"
#include "graphique.h"
#include "parametre.h"
#include "fenetre.h"
#include "loading_bar.h"
#include "perso.h"


class world : public QWidget
{
    Q_OBJECT
public:
    Fenetre *fenetre;

    QLabel *last_save;
    QPushButton *Save;
    QPushButton *Load;

    QLabel *date;
    QRadioButton *tpsreal;
    QCheckBox *Pause;

    QLabel *Delai;
    QDoubleSpinBox *Delai_input;

    QPushButton *Debug;

    QLabel* Create;
    QComboBox* Parametre;
    QPushButton *Map;
    QPushButton *Graphique;
    QPushButton *Persos;

    QGroupBox *Avancer;
    QPushButton *Tranche;
    QPushButton *Jour;
    QPushButton *Semaine;
    QPushButton *Mois;

    QSpinBox *AvanceChoose;
    QPushButton *AvancePerso;

    QString path;
    QString path_propertie;
    QString path_map;
    QString path_setup;


    compilateur *compil;

    ParaMesure *paramesure;
    ParaVirus *paravirus;
    bool need_update;

    Simulateur *simulateur;

    Graph *graphs;

    Mapping *map;
    QLabel tampon;
    QGraphicsBlurEffect floutage;

    Perso *personnages;

    loading_bar *loading;



    preferences *SETTINGS;

public:


    void crash(QString raison);
    void actualise();
    void layout();
    bool setup(compilateur *compil);
    world(preferences *point_set);
    ~world();
    FILE* check_file(const char *pathing);
    FILE* check_file(const QString pathing);

public slots:

    void parafun(int index);
    void update_need();
    void newgraph();
    void newmap();
    void newpersos();
    void silent_tranche();
    void tranche();
    void jour();
    void semaine();
    void mois();
    void perso();
    void real_time();
    void tranches(unsigned int n);
    void showdebug();
    void actualise_map(int position);
    void actualise_graph(int position);
    void actualise_target_graph(int position);
    void actualise_soft_graph(int position);
    void actualise_perso(int position);
    void destroy();
    void load();
    void save();
    void extract_graph(int position);
    void extract_map(int position);
    void map_clicked(int position);

private:
    void enabling();
    void disabling();


};



#endif // WORLD_H
