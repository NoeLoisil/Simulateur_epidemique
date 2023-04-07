#ifndef MAP_H
#define MAP_H

#include "fenetre.h"
#include "compilateur.h"
#include "clickable_label.h"
class Mapping : public QWidget
{
    Q_OBJECT
public:

    bool retracted;

    Fenetre *fenetre;
    QImage *image;
    ClickableLabel *supimage;
    QScrollBar *Vscrbar;
    QGridLayout *MAPSCR;
    QScrollBar *Hscrbar;
    QCheckBox *auto_up;
    QPushButton *update;
    QComboBox *cible;
    QPushButton *retract;
    QPushButton *upact;
    QVBoxLayout *upactlayout;
    QLabel *zoom;
    QLabel *seuil;
    QSlider *Szoom;
    QSlider *Sseuil;
    QGroupBox *legend;
    QLabel *info_pos;
    QLabel *info_population;
    QLabel *info_proxi;
    QLabel *info_contamination;

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

    MapType type;

    Mapping();
    ~Mapping();
    void layout(int position,compilateur *comp,const QSize size);

public slots:

    void destroy();
    void destroy_all();
    void Updatepressed();
    void Switchmode();
    void Switchtype(int type);
    void Updatezoom(int newzoom);
    void need_extract();
    void map_pressed();
signals:
    void clicked(int position);
    void actualise(int position);
    void extraction(int position);
};

#endif // MAP_H
