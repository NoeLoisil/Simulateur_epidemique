#include "perso.h"

#include "graphique.h"

Perso::Perso()
{
    fenetre = NULL;
    next = NULL;
    previous = NULL;
    index = 0;
    origin = NULL;

    go = NULL;
    id_choose = NULL;
    lucky = NULL;

    infos = NULL;
    id = NULL;
    nom = NULL;
    age = NULL;
    maladie = NULL;
    chance = NULL;

    bar = NULL;

    tuer = NULL;
    infecter = NULL;
    modifier = NULL;

    famille = NULL;

    map = NULL;
    supmap = NULL;
    factor = 1;
}

Perso::~Perso()
{
    if(fenetre != NULL)
    {
        fenetre->hide();

        delete go;
        delete id_choose;
        delete lucky;


        delete id;
        delete nom;
        delete age;
        delete maladie;
        delete chance;

        delete bar;

        delete tuer;
        delete infecter;
        delete modifier;

        delete map;
        delete supmap;


        for(int i = 0;i != 10;++i)
            delete id_famille[i];
        delete famille;
        delete infos;
        delete fenetre;
    }
}

void Perso::layout(int position,compilateur *comp,Simulateur *sim)
{
    compil = comp;
    index = position;
    simul = sim;
    fenetre = new Fenetre();
    fenetre->setBaseSize(485,418);
    go = new QPushButton(fenetre);
    go->setGeometry(10,10,31,23);
    go->setText("Go");
    id_choose = new QSpinBox(fenetre);
    id_choose->setGeometry(50,10,91,22);
    id_choose->setRange(0,simul->nb-1);
    lucky = new QPushButton(fenetre);
    lucky->setGeometry(150,10,121,23);
    lucky->setText("Je me sens chanceux");

    infos = new QGroupBox(fenetre);
    infos->setTitle("Informations :");
    infos->setGeometry(0,50,481,371);

    id = new QLabel(infos);
    id->setGeometry(20,20,400,13);
    nom = new QLabel(infos);
    nom->setGeometry(20,40,400,13);
    age = new QLabel(infos);
    age->setGeometry(20,60,400,13);
    maladie = new QLabel(infos);
    maladie->setGeometry(20,80,400,13);
    chance = new QLabel(infos);
    chance->setGeometry(20,100,400,13);

    bar = new QProgressBar(infos);
    bar->setGeometry(20,200,451,23);
    bar->setRange(0,100);
    bar->setValue(100);

    factor = max(simul->width/221+1,simul->height/181+1);
    supmap = new QLabel(infos);
    supmap->setGeometry(250,10,simul->width/factor,simul->height/factor);
    map = new QImage(simul->width/factor,simul->height/factor,QImage::Format::Format_RGB32);

    tuer = new QPushButton(fenetre);
    tuer->setGeometry(380,350,75,23);
    tuer->setText("Tuer");
    infecter = new QPushButton(fenetre);
    infecter->setGeometry(380,320,75,23);
    infecter->setText("Infecter");
    modifier = new QPushButton(fenetre);
    modifier->setGeometry(380,380,75,23);
    modifier->setText("Modifier");

    famille = new QGroupBox(fenetre);
    famille->setGeometry(20,280,231,121);
    famille->setTitle("Famille :");
    for(int i = 0;i != 10;++i)
    {
        id_famille[i] = new QLabel(famille);
        id_famille[i]->setGeometry(10+(i/5)*110,20+(i%5)*20,100,13);
        id_famille[i]->setText("123456");
    }

    char *gblanket = challoc(32);
    sprintf(gblanket,"Perso %d",index);
    fenetre->setWindowTitle(gblanket);
    free(gblanket);
    fenetre->show();
    connect(fenetre,SIGNAL(closing()),this,SLOT(destroy()));
    connect(go,SIGNAL(clicked()),this,SLOT(change_id()));
    connect(id_choose,SIGNAL(valueChanged(int)),this,SLOT(change_id(int)));
    connect(lucky,SIGNAL(clicked()),this,SLOT(change_id_lucky()));
    connect(tuer,SIGNAL(clicked()),this,SLOT(kill()));
    connect(infecter,SIGNAL(clicked()),this,SLOT(infecte()));

    actual_id = 0;
}

void Perso::destroy()
{
    compil->add("destroy perso num ",index);
    if(previous != NULL)
        ((Perso *)previous)->next = next;
    else
        *origin = next;
    if(next != NULL)
        ((Perso *)next)->previous = previous;

    Perso *p = (Perso *)next;
    while(p != NULL)
    {
        p->index--;
        p = (Perso *)p->next;
    }
    delete this;
}

void Perso::destroy_all()
{
    if(next != NULL)
        ((Perso *)next)->destroy_all();
    delete this;
}

void Perso::change_id(int _)
{
    actual_id = id_choose->value();
    emit actualise(index);
}

void Perso::change_id_lucky()
{
    float relative = id_choose->value();
    if(relative == 0 || relative == 1)
        relative = 0.45;
    relative /= simul->nb;
    for(int i = 0;i != 25;++i)
        relative = 4.*relative*(1.-relative);
    actual_id = relative*simul->nb;
    id_choose->setValue(actual_id);
    emit actualise(index);
}

void Perso::kill()
{
    simul->population[actual_id].dead = true;
    simul->population[actual_id].maladie = -1;
    emit actualise(index);
}

void Perso::infecte()
{
    simul->population[actual_id].maladie = simul->temps_infection;
#if false // test
    for(long i = 0;i != simul->nb;++i)
        simul->population[i].maladie = simul->temps_infection;
#endif
                emit actualise(index);
}
