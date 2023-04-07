#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include "fenetre.h"
#include "compilateur.h"


class Graph : public QWidget
{
    Q_OBJECT
public:

    bool retracted;

    Fenetre *fenetre;
    QCustomPlot *plot;
    QCheckBox *auto_up;
    QPushButton *update;
    QComboBox *cible;
    QPushButton *retract;
    QPushButton *upact;
    QVBoxLayout *upactlayout;
    QLabel *zonetemporelle;
    QSlider *Szonetemporelle;
    QCheckBox *loga;
    QGroupBox *legend;
    QHBoxLayout *HBL;
    QVBoxLayout *MAPRIGHT;
    QWidget *cache;
    QHBoxLayout *UP1;
    QHBoxLayout *UP2;
    QHBoxLayout *UP3;
    QHBoxLayout *UP4;
    QHBoxLayout *UP5;
    void *next;
    void *previous;
    int index;
    void **origin;
    compilateur *compil;

    QPushButton *extract;

    GraphType type;
    int old_time;
    void resize_plot(int new_time);

    unsigned long previous_time_data = 0;

    Graph();
    ~Graph();
    void layout(int position,compilateur *comp);


public slots:

    void destroy();
    void destroy_all();
    void Updatepressed();
    void Switchmode();
    void Switchtype(int type);
    void update_time(int newindex);
    void Logapressed();
    void need_extract();
signals:
    void extraction(int position);
    void actualise(int position);
    void actualise_target(int position);
};


#endif // GRAPHIQUE_H
