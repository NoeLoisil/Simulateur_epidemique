#include "parametre.h"

ParaMesure::ParaMesure()
{
    fenetre = NULL;
    teletravail = NULL;
    distanciation = NULL;
    nbmasque = NULL;
    effmasque = NULL;
    vaccinationjournaliere = NULL;
    autoconfinement = NULL;
    hopital_re = NULL;
    ecole = NULL;
    commerce = NULL;
    industriel = NULL;
    loisir = NULL;
    container = NULL;
    layoutcontainer = NULL;
    layoutparameter = NULL;
    layoutfermeture = NULL;
}
ParaMesure::~ParaMesure()
{

}
void ParaMesure::setup(Simulateur *sim)
{
    simul = sim;
    fenetre = new QScrollArea();
    fenetre->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    fenetre->setWindowTitle("Paramètre des mesures et comportements");
    fenetre->setFixedWidth(610);
    container = new QWidget();
    container->setFixedSize(580,1000);
    layoutcontainer = new QVBoxLayout(container);
    layoutparameter = new QGridLayout();
    layoutfermeture = new QVBoxLayout();
    layoutcontainer->addLayout(layoutparameter);
    layoutcontainer->addLayout(layoutfermeture);
    fenetre->setWidget(container);
    teletravail = new Parameter();
    teletravail->setText("Utilisation du télétravail ( %d%% )");
    distanciation = new Parameter();
    distanciation->setText("Respect de la distanciation sociale ( %d%% )");
    nbmasque = new Parameter();
    nbmasque->setText("Port du masque ( %d%% )");
    effmasque = new Parameter();
    effmasque->setText("Efficacité du masque ( %d%% )");
    vaccinationjournaliere = new Parameter();
    vaccinationjournaliere->setloga(true,12);
    vaccinationjournaliere->setRange(-1,60);
    vaccinationjournaliere->setText("Vaccination par tranche ( %d )");
    autoconfinement = new Parameter();
    autoconfinement->setText("Auto-confinement ( %d%% )");
    hopital_re = new Parameter();
    hopital_re->setloga(true,12);
    hopital_re->setRange(-1,30);
    hopital_re->setText("Capacite_hopital ( %d%% )");

    ecole = new SliderFermeture();
    ecole->setTitle("Ouverture des écoles");
    ecole->setDescription("Réglage de l'ouverture des écoles ( fermé par défaut entre 18h et 8h )");
    commerce = new SliderFermeture();
    commerce->setTitle("Ouverture des commerces");
    commerce->setDescription("Réglage de l'ouverture des commerces");
    industriel = new SliderFermeture();
    industriel->setTitle("Ouverture des zones industrielles");
    industriel->setDescription("Réglage de l'ouverture des zones industrielles");
    loisir = new SliderFermeture();
    loisir->setTitle("Ouvertures des loisirs");
    loisir->setDescription("Réglage de l'ouverture des loisirs");


    layoutparameter->addWidget(teletravail,0,0);
    layoutparameter->addWidget(distanciation,0,1);
    layoutparameter->addWidget(nbmasque,1,0);
    layoutparameter->addWidget(effmasque,1,1);
    layoutparameter->addWidget(vaccinationjournaliere,2,0);
    layoutparameter->addWidget(autoconfinement,3,0);
    layoutfermeture->addWidget(ecole);
    layoutfermeture->addWidget(commerce);
    layoutfermeture->addWidget(industriel);
    layoutfermeture->addWidget(loisir);

    connect(teletravail,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(distanciation,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(nbmasque,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(effmasque,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(vaccinationjournaliere,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(autoconfinement,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(hopital_re,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(ecole,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(commerce,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(industriel,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(loisir,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
}

ParaVirus::ParaVirus()
{
    fenetre = NULL;
    VLayout = NULL;
    efficacite_zone = NULL;
    distance_zone = NULL;
    efficacite_famille = NULL;
    facteur_degat = NULL;
    temps_infection = NULL;
    debut_infecteux = NULL;
    temps_infecteux = NULL;
    effvaccin = NULL;
    nouveau = NULL;
    factor_distance = NULL;
}
ParaVirus::~ParaVirus()
{

}
void ParaVirus::setup(Simulateur *sim)
{
    simul = sim;
    fenetre = new QWidget();
    fenetre->setWindowTitle("Paramètre du virus");
    //fenetre->setFixedSize(500,500);
    VLayout = new QVBoxLayout(fenetre);

    efficacite_zone = new Parameter();
    efficacite_zone->setText("Propagation du virus ( %d%% )");
    efficacite_zone->setRange(0,1000);
    efficacite_zone->setFixedSize(500,40);
    efficacite_zone->slider->setGeometry(0,20,450,10);
    distance_zone = new Parameter();
    distance_zone->setText("Distance d'infectueusité du virus ( %d )");
    distance_zone->setRange(1,50);
    distance_zone->setFixedSize(500,40);
    distance_zone->slider->setGeometry(0,20,450,10);



    efficacite_famille = new Parameter();
    efficacite_famille->setText("Propagation du virus en famille ( %d%% )");
    efficacite_famille->setRange(0,1000);
    efficacite_famille->setFixedSize(500,40);
    efficacite_famille->slider->setGeometry(0,20,450,10);
    facteur_degat = new Parameter();
    facteur_degat->setText("Dégat du virus (%d%%)");
    facteur_degat->setloga(true,80);
    facteur_degat->setRange(-1,320);
    facteur_degat->setFixedSize(500,40);
    facteur_degat->slider->setGeometry(0,20,450,10);
    temps_infection = new Parameter();
    temps_infection->setText("Durée de la contamination ( %d )");
    temps_infection->setloga(true,242);
    temps_infection->setRange(-1,851);
    temps_infection->setFixedSize(500,40);
    temps_infection->slider->setGeometry(0,20,450,10);
    debut_infecteux = new Parameter();
    debut_infecteux->setText("Temps avant de devenir infectueux ( %d )");
    debut_infecteux->setRange(-1,189);
    debut_infecteux->setFixedSize(500,40);
    debut_infecteux->slider->setGeometry(0,20,450,10);
    temps_infecteux = new Parameter();
    temps_infecteux->setText("Temps infectueux ( %d )");
    temps_infecteux->setRange(-1,189);
    temps_infecteux->setFixedSize(500,40);
    temps_infecteux->slider->setGeometry(0,20,450,10);
    effvaccin = new Parameter();
    effvaccin->setText("Efficacité du vaccin ( %d%% )");
    effvaccin->setRange(-1,100);
    effvaccin->setFixedSize(500,40);
    effvaccin->slider->setGeometry(0,20,450,10);
    nouveau = new Parameter();
    nouveau->setText("contamination exterieur par tranche ( %d )");
    nouveau->setloga(true,48);
    nouveau->setRange(-1,240);
    nouveau->setFixedSize(500,40);
    nouveau->slider->setGeometry(0,20,450,10);
    factor_distance = new Parameter();
    factor_distance->setText("facteur distanciel ( manhattan ) ( %d%% )");
    factor_distance->setRange(0,100);
    factor_distance->setFixedSize(500,40);
    factor_distance->slider->setGeometry(0,20,450,10);

    VLayout->addWidget(distance_zone);
    VLayout->addWidget(efficacite_zone);
    VLayout->addWidget(efficacite_famille);
    VLayout->addWidget(facteur_degat);
    VLayout->addWidget(temps_infection);
    VLayout->addWidget(debut_infecteux);
    VLayout->addWidget(temps_infecteux);
    VLayout->addWidget(effvaccin);
    VLayout->addWidget(nouveau);
    VLayout->addWidget(factor_distance);

    connect(efficacite_famille,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(effvaccin,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(facteur_degat,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(efficacite_zone,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(distance_zone,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(temps_infection,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(debut_infecteux,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(temps_infecteux,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(nouveau,SIGNAL(will_need_update()),this,SLOT(Update_passif()));
    connect(factor_distance,SIGNAL(will_need_update()),this,SLOT(Update_passif()));

    connect(facteur_degat,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));
    connect(distance_zone,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));
    connect(temps_infection,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));
    connect(debut_infecteux,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));
    connect(temps_infecteux,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));
    connect(factor_distance,SIGNAL(will_need_update()),this,SLOT(Soon_Update()));


}

void ParaVirus::setsimulateur(Simulateur *sim,compilateur *comp)
{
    compil = comp;
    simul = sim;
    Update_passif();
    Update_actif();
}

void ParaMesure::setsimulateur(Simulateur *sim,compilateur *comp)
{
    compil = comp;
    simul = sim;
    Update_passif();
}

void ParaMesure::Update_passif(int _)
{
    simul->teletravail = teletravail->value();
    simul->distanciation = distanciation->value();
    simul->nbmasque = nbmasque->value();
    simul->effmasque = effmasque->value();
    simul->vaccinationjournaliere = vaccinationjournaliere->value();
    simul->autoconfinement = autoconfinement->value();
    simul->notautoconfinement = 100-autoconfinement->value();
    simul->hopital_re = hopital_re->value();
    ecole->apply(&simul->ecole);
    industriel->apply(&simul->industriel);
    commerce->apply(&simul->commerce);
    loisir->apply(&simul->loisir);
}

void ParaVirus::Update_passif(int _)
{
    simul->efficacite_famille = efficacite_famille->value();
    simul->effvaccin = effvaccin->value();
    simul->facteur_degat = facteur_degat->value()*189./temps_infection->value()*0.01;
    simul->efficacite_zone = efficacite_zone->value();
    simul->nouveau = nouveau->value();
    temps_infecteux->setRange(0,temps_infection->value());
    temps_infecteux->setValue(borne(temps_infecteux->value(),0,temps_infection->value()));
    debut_infecteux->setRange(0,temps_infection->value());
    debut_infecteux->setValue(borne(debut_infecteux->value(),0,temps_infection->value()));
    simul->temps_infecteux = temps_infecteux->value();
    simul->temps_infection = temps_infection->value();
    simul->debut_infecteux = debut_infecteux->value();
    simul->factor_distance = factor_distance->value();
    simul->distance_zone = distance_zone->value();
}

void ParaVirus::Soon_Update(int _)
{
    emit Update_need();
}

void ParaVirus::Update_actif(int _)
{
    int start_infect = simul->temps_infection-simul->debut_infecteux;
    int end_infect = simul->temps_infection-simul->debut_infecteux-simul->temps_infecteux;

    if(simul->tranche_actuel == 0 && simul->jour == 0)
    {
        for(long i = 0;i != simul->nb;++i)
            if(simul->population[i].maladie == 1)
                simul->population[i].maladie = simul->temps_infection;
    }


    if(end_infect < 0 && start_infect >= 0)
        end_infect = 0;
    long ind = 0;
    for(long g = 0; g != 255;++g)
        for(long t = 0;t != 189;t++)
            simul->precalcdegat[ind++] = simul->degat(g,t);

    if(simul->distanciel != NULL)
        free(simul->distanciel);
    simul->distanciel = new Uint64[2*simul->distance_zone+1];

    Uint64 h = 1 << (2*simul->distance_zone+1);
    for(int i = 0;i != 2*simul->distance_zone+1;++i)
    {
        simul->distanciel[i] = h;
        h = h*simul->factor_distance/100;
    }

    simul->counter_commerce.reset();
    simul->counter_densite.reset();
    simul->counter_habitation.reset();
    simul->counter_industriel.reset();
    simul->counter_loisir.reset();
    simul->counter_school.reset();

    for(int i = 0;i != simul->nb;++i)
    {
        if(simul->population[i].maladie >= end_infect && simul->population[i].maladie < start_infect)
        {
            simul->addcontamine(&simul->population[i],1);
        }
        simul->counter_densite.add(simul->population[i].hab_x,simul->population[i].hab_y,1,simul->distance_zone,simul->distanciel);
    }

    for(int i = 0;i !=simul->imsize;++i)
    {
        double d = simul->counter_densite.data[i];
        if(d)
            simul->inverse_densite[i] = 1./d;
        else
            simul->inverse_densite[i] = 4294967296.;
    }
}

void ParaMesure::reset(const QString path)
{
    FILE* file = fopen(path.toStdString().c_str(),"r");
    teletravail->setValue(take_long(file));
    distanciation->setValue(take_long(file));
    nbmasque->setValue(take_long(file));
    effmasque->setValue(take_long(file));
    vaccinationjournaliere->setValue(take_long(file));
    autoconfinement->setValue(take_long(file));
    hopital_re->setValue(take_long(file));
    ecole->setValues(file);
    commerce->setValues(file);
    industriel->setValues(file);
    loisir->setValues(file);
    fclose(file);
    Update_passif();
}

void ParaVirus::reset(const QString path)
{
    FILE* file = fopen(path.toStdString().c_str(),"r");
    efficacite_zone->setValue(take_long(file));
    distance_zone->setValue(take_long(file));
    efficacite_famille->setValue(take_long(file));
    facteur_degat->setValue(take_long(file));
    temps_infection->setValue(take_long(file));
    debut_infecteux->setValue(take_long(file));
    temps_infecteux->setValue(take_long(file));
    effvaccin->setValue(take_long(file));
    nouveau->setValue(take_long(file));
    factor_distance->setValue(take_long(file));
    fclose(file);
    Update_passif();
    Update_actif();
}
