#include "world.h"
#include <windows.h>
#include <QtGui>

extern preferences SETTINGS;

world::world(preferences *point_set)
{
    SETTINGS = point_set;
    fenetre = NULL;

    last_save = NULL;
    Save = NULL;
    Load = NULL;

    date = NULL;
    tpsreal = NULL;
    Pause = NULL;

    Delai = NULL;
    Delai_input = NULL;

    Debug = NULL;

    Create = NULL;
    Parametre = NULL;
    Map = NULL;
    Graphique = NULL;

    Avancer = NULL;
    Tranche = NULL;
    Jour = NULL;
    Semaine = NULL;
    Mois = NULL;
    AvanceChoose = NULL;
    AvancePerso = NULL;

    path = NULL;
    path_propertie = NULL;
    path_map = NULL;

    compil = NULL;

    paramesure = NULL;
    paravirus = NULL;

    simulateur = NULL;

    graphs = NULL;
    map = NULL;
    personnages = NULL;

    loading = NULL;
}

void world::layout()
{
     fenetre = new Fenetre();
     fenetre->setFixedSize(330,240);
     fenetre->setImportant(true);
     fenetre->setWindowTitle("Gestionnaire");
     last_save = new QLabel(fenetre);
     last_save->move(10,10);
     last_save->setText("Dernière Sauvegarde : 22 janvier 2020 12h67");
     Save = new QPushButton(fenetre);
     Save->move(10,40);
     Save->setText("Save");
     Save->setEnabled(false);
     connect(Save,SIGNAL(clicked()),this,SLOT(save()));
     Load = new QPushButton(fenetre);
     Load->move(10,70);
     Load->setText("Load");
     connect(Load,SIGNAL(clicked()),this,SLOT(load()));
     date = new QLabel(fenetre);
     date->move(130,30);
     date->setText("Jour 99999 : 12h00");
     tpsreal = new QRadioButton(fenetre);
     tpsreal->move(80,100);
     tpsreal->setText("Temps réel");
     Pause = new QCheckBox(fenetre);
     Pause->move(110,70);
     Pause->setText("Pause");
     Pause->setChecked(false);


     Delai = new QLabel(fenetre);
     Delai->move(40,120);
     Delai->setText("Delai (s)");
     Delai_input = new QDoubleSpinBox(fenetre);
     Delai_input->move(100,120);
     Delai_input->setDecimals(3);
     Delai_input->setSingleStep(0.001);
     Delai_input->setRange(0.,1.);

     connect(tpsreal,SIGNAL(clicked()),this,SLOT(real_time()));
     connect(Pause,SIGNAL(clicked()),this,SLOT(real_time()));

     Debug = new QPushButton(fenetre);
     Debug->setText("Debug");
     Debug->move(10,150);
     connect(Debug,SIGNAL(clicked()),this,SLOT(showdebug()));
     Create = new QLabel(fenetre);
     Create->move(10,180);
     Create->setText("Créer :");
     Parametre = new QComboBox(fenetre);
     Parametre->move(0,210);
     Parametre->addItem("Paramètres");
     Parametre->addItem("Mesures et comportements");
     Parametre->addItem("Virus");
     connect(Parametre,SIGNAL(currentIndexChanged(int)),this,SLOT(parafun(int)));
     Map = new QPushButton(fenetre);
     Map->move(170,210);
     Map->setText("Map");
     connect(Map,SIGNAL(clicked()),this,SLOT(newmap()));
     Graphique = new QPushButton(fenetre);
     Graphique->move(240,210);
     Graphique->setText("Graphique");
     connect(Graphique,SIGNAL(clicked()),this,SLOT(newgraph()));
     Persos = new QPushButton(fenetre);
     Persos->move(82,185);
     Persos->setText("Persos");
     connect(Persos,SIGNAL(clicked()),this,SLOT(newpersos()));
     Avancer = new QGroupBox(fenetre);
     Avancer->setFixedSize(120,170);
     Avancer->move(180,40);
     Avancer->setTitle("Avancer:");
     Tranche = new QPushButton(Avancer);
     Tranche->setText("Tranche");
     Tranche->move(10,20);
     connect(Tranche,SIGNAL(clicked()),this,SLOT(tranche()));
     Jour = new QPushButton(Avancer);
     Jour->setText("Jour");
     Jour->move(10,40);
     connect(Jour,SIGNAL(clicked()),this,SLOT(jour()));
     Semaine = new QPushButton(Avancer);
     Semaine->setText("Semaine");
     Semaine->move(10,60);
     connect(Semaine,SIGNAL(clicked()),this,SLOT(semaine()));
     Mois = new QPushButton(Avancer);
     Mois->setText("Mois");
     Mois->move(10,80);
     AvanceChoose = new QSpinBox(Avancer);
     AvanceChoose->setRange(0,3285000);
     AvanceChoose->move(11,111);
     AvancePerso = new QPushButton(Avancer);
     AvancePerso->setText("Avancer");
     AvancePerso->move(10,130);
     connect(AvancePerso,SIGNAL(clicked()),this,SLOT(perso()));
     connect(Mois,SIGNAL(clicked()),this,SLOT(mois()));
     connect(fenetre,SIGNAL(closing()),this,SLOT(destroy()));
     paramesure = new ParaMesure();
     paravirus = new ParaVirus();
     disabling();

}

void world::enabling()
{
    tpsreal->setEnabled(true);
    Pause->setEnabled(true);
    Parametre->setEnabled(true);
    Map->setEnabled(true);
    Graphique->setEnabled(true);
    Persos->setEnabled(true);
    Tranche->setEnabled(true);
    Jour->setEnabled(true);
    Semaine->setEnabled(true);
    Mois->setEnabled(true);
    AvancePerso->setEnabled(true);
}

void world::disabling()
{
    tpsreal->setEnabled(false);
    Pause->setEnabled(false);
    Parametre->setEnabled(false);
    Map->setEnabled(false);
    Graphique->setEnabled(false);
    Persos->setEnabled(false);
    Tranche->setEnabled(false);
    Jour->setEnabled(false);
    Semaine->setEnabled(false);
    Mois->setEnabled(false);
    AvancePerso->setEnabled(false);
    if(graphs)
        graphs->destroy_all();
    if(map)
        map->destroy_all();
    if(personnages)
        personnages->destroy_all();
    graphs = NULL;
    map = NULL;
}

void world::actualise()
{
    if(simulateur)
        date->setText(simulateur->getDate());
    else
        date->setText("pas de simulation en cours...");
    date->adjustSize();
    last_save->setText("Dernière sauvegarde : Jamais");
    last_save->adjustSize();
    Graph *p = graphs;
    unsigned int i = 0;
    while(p != NULL)
    {
        if(p->auto_up->isChecked())
            actualise_soft_graph(i);
        p = (Graph *)p->next; i++;
    }
    Mapping *m = map;
    i = 0;
    while(m != NULL)
    {
        if(m->auto_up->isChecked())
            actualise_map(i);
        m = (Mapping *)m->next; i++;
    }
    Perso *pe = personnages;
    i = 0;
    while(pe != NULL)
    {
        actualise_perso(i);
        pe = (Perso *)pe->next; i++;
    }
}

void world::crash(QString raison)
{
    QMessageBox* Box = new QMessageBox();
    Box->critical(compil->fenetre,"plantage",raison);
    delete Box;
}

FILE* world::check_file(const QString pathing)
{
    FILE* file = tfopen(pathing.toStdString().c_str());
    if(file == NULL)
    {
        compil->add("Fichier \"");
        compil->add(pathing);
        compil->add("\" manquant\n");
        crash("Crash lors de la création, verifiez vos fichiers");
        delete this;
        compil->add("La mémoire a bien été libéré lors du crash\n");
        return NULL;
    }
    else
    {
        compil->add("Fichier \"");
        compil->add(pathing);
        compil->add("\" ouvert\n");
    }
    return file;
}

FILE* world::check_file(const char *pathing)
{
    FILE* file = tfopen(pathing);
    if(file == NULL)
    {
        compil->add("Fichier \"");
        compil->add(pathing);
        compil->add("\" manquant\n");
        crash("Crash lors de la création, verifiez vos fichiers");
        delete this;
        compil->add("La mémoire a bien été libéré lors du crash\n");
        return NULL;
    }
    else
    {
        compil->add("Fichier \"");
        compil->add(pathing);
        compil->add("\" ouvert\n");
    }
    return file;
}

bool world::setup(compilateur *Pcompil)
{
    compil = Pcompil;
    loading = new loading_bar();
    compil->fenetre->hide();
    layout();
    paramesure->setup();
    paravirus->setup();
    simulateur = NULL;
    need_update = false;
    fenetre->show();
    actualise();
    return 0;
}

void world::load()
{
    /*
    QMessageBox* Box = new QMessageBox();
    QMessageBox::StandardButton reply = Box->question(compil->fenetre,"Ouvrir une sauvegarde ?","Pour ouvrir une sauvegarde, cliquez sur Oui, pour ouvrir un monde, cliquez sur Non");
    delete Box;
    */
    QMessageBox::StandardButton reply = QMessageBox::No;
    if(reply == QMessageBox::No)
    {
        SETTINGS->load();
        QString dossier = QFileDialog::getExistingDirectory(nullptr,QString(),SETTINGS->world_file);
        SETTINGS->world_file = dossier.toStdString().c_str();
        SETTINGS->save();
        clock_t stop,start = clock();
        compil->add("Début de la création...\n");
        stop = clock();
        int s = (stop-start)/CLOCKS_PER_SEC;
        int ms = (((stop-start)*1000)/CLOCKS_PER_SEC)-(s*1000);
        path_propertie = dossier+"/propertie.txt";
        path_setup = dossier+"/setup.txt";
        FILE* file_prop = check_file(path_propertie);
        FILE* file_setup = check_file(path_setup);
        if(!simulateur) delete simulateur;
        long seed = take_long(file_setup);
        char *path_map = challoc(256);
        char *path_mesure = challoc(256);
        char *path_virus = challoc(256);
        take_chaine(file_setup,path_map,-1);
        take_chaine(file_setup,path_mesure,-1);
        take_chaine(file_setup,path_virus,-1);
        simulateur = new Simulateur(file_setup,compil,loading,dossier,seed,path_map);
        paramesure->setsimulateur(simulateur,compil);
        paravirus->setsimulateur(simulateur,compil);
        paramesure->reset(dossier+"/"+path_mesure);
        paravirus->reset(dossier+"/"+path_virus);
        connect(paravirus,SIGNAL(Update_need()),this,SLOT(update_need()));
        char* blanket = challoc(256);
        sprintf(blanket,"Création terminé, temps de création : %d.%0*d seconde(s)\n",s,3,ms);
        compil->add(blanket);
        free(blanket);
        fclose(file_setup);
        fclose(file_prop);

        enabling();
    }
}

void world::save()
{

}

void world::parafun(int index)
{
    switch(index)
    {
        case PARA_MESURE:
            paramesure->fenetre->show();
            break;
        case PARA_VIRUS:
            paravirus->fenetre->show();
            break;
    }
    Parametre->setCurrentIndex(0);
}

void world::newgraph()
{
    int i = 0;
    Graph *p = graphs;
    if(p == NULL)
    {
        graphs = new Graph();
        graphs->layout(0,compil);
        graphs->previous = NULL;
        p = graphs;
    }
    else
    {
        i = 1;
        while(p->next != NULL)
        {
            p = (Graph *)p->next;
            ++i;
        }
        p->next = new Graph();
        ((Graph *)p->next)->previous = p;
        p = (Graph *)p->next;
        p->layout(i,compil);
    }
    p->origin = (void **)&graphs;
    connect(p,SIGNAL(actualise(int)),this,SLOT(actualise_graph(int)));
    connect(p,SIGNAL(actualise_target(int)),this,SLOT(actualise_target_graph(int)));
    connect(p,SIGNAL(extraction(int)),this,SLOT(extract_graph(int)));
    actualise_graph(i);
}

void world::newmap()
{
    int i = 0;
    Mapping *p = map;
    if(p == NULL)
    {
        map = new Mapping();
        map->layout(0,compil,simulateur->im->size());
        map->previous = NULL;
        p = map;
    }
    else
    {
        i = 1;
        while(p->next != NULL)
        {
            p = (Mapping *)p->next;
            ++i;
        }
        p->next = new Mapping();
        ((Mapping *)p->next)->previous = p;
        p = (Mapping *)p->next;
        p->layout(i,compil,simulateur->im->size());
    }
    p->origin = (void **)&map;
    connect(p,SIGNAL(actualise(int)),this,SLOT(actualise_map(int)));
    connect(p,SIGNAL(extraction(int)),this,SLOT(extract_map(int)));
    connect(p,SIGNAL(clicked(int)),this,SLOT(map_clicked(int)));
    actualise_map(i);
}

void world::newpersos()
{
    int i = 0;
    Perso *p = personnages;
    if(p == NULL)
    {
        personnages = new Perso();
        personnages->layout(0,compil,simulateur);
        personnages->previous = NULL;
        p = personnages;
    }
    else
    {
        i = 1;
        while(p->next != NULL)
        {
            p = (Perso *)p->next;
            ++i;
        }
        p->next = new Perso();
        ((Perso *)p->next)->previous = p;
        p = (Perso *)p->next;
        p->layout(i,compil,simulateur);
    }
    p->origin = (void **)&personnages;
    connect(p,SIGNAL(actualise(int)),this,SLOT(actualise_perso(int)));
    actualise_perso(i);
}

void world::silent_tranche()
{
    if(need_update)
    {
        paravirus->Update_actif();
        need_update = false;
    }
    simulateur->tranche();
    actualise();
}

void world::tranche()
{
    if(need_update)
    {
        paravirus->Update_actif();
        need_update = false;
    }
    compil->add("calcul d'une tranche...\n");
    clock_t start = clock();
    simulateur->tranche();
    clock_t stop = clock();
    actualise();
    clock_t stop_refresh = clock();
    int s = (stop-start)/CLOCKS_PER_SEC;
    int ms = (((stop-start)*1000)/CLOCKS_PER_SEC)-(s*1000);
    int sr = (stop_refresh-stop)/CLOCKS_PER_SEC;
    int msr = (((stop_refresh-stop)*1000)/CLOCKS_PER_SEC)-(sr*1000);
    char *blanket = challoc(256);
    sprintf(blanket,"calcul terminé, temps de calcul : %d.%0*d seconde(s)\n",s,3,ms);
    compil->add(blanket);
    sprintf(blanket,"calcul gpu terminé, temps total : %d.%0*d seconde(s)\n",sr,3,msr);
    compil->add(blanket);
    free(blanket);
}

void world::jour()
{
    tranches(9);
    actualise();
}

void world::semaine()
{
    tranches(63);
    actualise();
}

void world::mois()
{
    tranches(270);
    actualise();
}

void world::perso()
{
    tranches(AvanceChoose->value());
    actualise();
}

void world::tranches(unsigned int n)
{
    if(need_update)
    {
        paravirus->Update_actif();
        need_update = false;
    }
    char *blanket = challoc(256);
    sprintf(blanket,"calcul de %d tranches...\n",n);
    compil->add(blanket);
    clock_t start = clock();
    simulateur->tranches(n);
    clock_t stop = clock();
    actualise();
    clock_t stop_refresh = clock();
    int s = (stop-start)/CLOCKS_PER_SEC;
    int ms = (((stop-start)*1000)/CLOCKS_PER_SEC)-(s*1000);
    int sr = (stop_refresh-stop)/CLOCKS_PER_SEC;
    int msr = (((stop_refresh-stop)*1000)/CLOCKS_PER_SEC)-(sr*1000);
    sprintf(blanket,"calcul terminé, temps de calcul : %d.%0*d seconde(s)\n",s,3,ms);
    compil->add(blanket);
    sprintf(blanket,"calcul gpu terminé, temps total : %d.%0*d seconde(s)\n",sr,3,msr);
    compil->add(blanket);
    free(blanket);
}

void world::showdebug()
{
    compil->fenetre->show();
}

Uint32 degrade_map(Uint16 value)
{
    if(value & 0x100)
        return 0xFF0000 | ((0x1FF-value)*0x100);
    else
        return 0xFF00 | (value*0x10000);
}

void world::actualise_map(int position)
{
    Mapping *p = map;
    for(int i = 0;i != position;++i)
        p = (Mapping *)p->next;
    char *blanket = challoc(16);
    sprintf(blanket,"Map %d",p->index);
    p->fenetre->setWindowTitle(blanket);
    free(blanket);

    uchar* pixels = p->image->bits();
    Uint32* pixels32 = (Uint32 *)pixels;
    long imsize = simulateur->imsize;
    long nb = simulateur->nb;
    switch(p->type)
    {
    case MAP_POPULATION:{
        p->image->fill(0xFF00);
        QVector<Uint64> counter(imsize);
        counter.fill(0);
        for(long i = 0;i != nb;++i)
            counter[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width]++;

        counter = Blur8(counter,p->Sseuil->value(),simulateur->width);
        Uint64 max = 1;
        for(long i = 0;i != imsize;++i)
            if(counter.at(i) > max) max = counter.at(i);
        for(long i = 0;i != imsize;++i)
            pixels32[i] = degrade_map(counter[i]*0x1FF/max);
        break;}
    case MAP_CONTAMINATION_ABS:{
        p->image->fill(0xFF00);
        QVector<Uint64> counter(imsize);
        counter.fill(0);
        for(long i = 0;i != nb;++i)
            if(simulateur->population[i].maladie > 0)
                counter[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width]++;
        counter = Blur8(counter,p->Sseuil->value(),simulateur->width);
        Uint64 max = 1;
        for(long i = 0;i != imsize;++i)
            if(counter.at(i) > max) max = counter.at(i);
        for(long i = 0;i != imsize;++i)
            pixels32[i] = degrade_map(counter[i]*0x1FF/max);
        break;}
    case MAP_CONTAMINATION_REL:{
        p->image->fill(0xFF00);
        QVector<Uint64> counter(imsize);
        QVector<Uint64> habit(imsize);
        counter.fill(0);
        habit.fill(0);
        for(long i = 0;i != nb;++i)
        {
            if(simulateur->population[i].maladie > 0)
                counter[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width] += 0x1FF;
            habit[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width]++;
        }
        for(long i = 0;i != imsize;++i)
            if(habit[i] != 0)
                counter[i] = counter[i]/habit[i];
        counter = Blur8(counter,p->Sseuil->value(),simulateur->width);
        for(long i = 0;i != imsize;++i)
            pixels32[i] = degrade_map(counter[i]*0x1FF/512);
        break;}
    case MAP_MORT:{
        p->image->fill(0xFF00);
        QVector<Uint64> counter(simulateur->imsize);
        QVector<Uint64> habit(simulateur->imsize);
        counter.fill(0);
        habit.fill(0);
        for(long i = 0;i != nb;++i)
        {
            if(simulateur->population[i].dead)
                counter[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width] += 0x1FF;
            habit[simulateur->population[i].hab_x+simulateur->population[i].hab_y*simulateur->width]++;
        }
        for(long i = 0;i != imsize;++i)
            if(habit[i] != 0)
                counter[i] = counter[i]/habit[i];
        counter = Blur8(counter,p->Sseuil->value(),simulateur->width);
        for(long i = 0;i != imsize;++i)
            pixels32[i] = degrade_map(counter[i]*0x1FF/512);
        break;}
    case MAP_PROXI:{
        p->image->fill(0xFF00);
        Uint64 max = 1;
        for(Sint32 i = 0;i != simulateur->imsize;++i)
            if(simulateur->counter_habitation.data[i] > max)
                max = simulateur->counter_habitation.data[i];
        for(long i = 0;i != imsize;++i)
            pixels32[i] = degrade_map(simulateur->counter_habitation.data[i]*0x1FF/max);
        break;}
    default:
        p->image->fill(0xFF00);
        break;
    }


    Uint32 *pixels_o = (Uint32 *)simulateur->im->bits();
    Uint32 black = pixels_o[0];
    for(long i = 0;i != imsize;++i)
        if(pixels_o[i] == black)
                pixels32[i] = 0x49A7FF;

    if(p->Szoom->value() >= 1)
        p->supimage->setPixmap(QPixmap::fromImage(*p->image).scaled(p->image->size()*p->Szoom->value()));
    else
        p->supimage->setPixmap(QPixmap::fromImage(*p->image).scaled(p->image->size()/(1 << (1-p->Szoom->value()))));
}

void world::actualise_graph(int position)
{
    Graph *p = graphs;
    for(int i = 0;i != position;++i)
        p = (Graph *)p->next;
    char *blanket = challoc(256);
    sprintf(blanket,"Graphique %d",p->index);
    p->fenetre->setWindowTitle(blanket);
    free(blanket);
    p->type = p->cible->currentIndex();
    while(p->plot->graphCount() != 0)
        p->plot->removeGraph(0);
    switch(p->type)
    {
    case GRAPH_CONTAMINATION_TOTAL:
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->graph(0)->setPen(QPen(Qt::black));
        p->plot->graph(1)->setPen(QPen(Qt::red));
        p->plot->graph(2)->setPen(QPen(Qt::blue));
        if(p->loga->checkState() == Qt::CheckState::Checked)
        {
            p->plot->graph(0)->setData(simulateur->all,simulateur->nb_mort.timeline(true));
            p->plot->graph(1)->setData(simulateur->all,simulateur->nb_infecte.timeline(true));
            p->plot->graph(2)->setData(simulateur->all,simulateur->nb_gueris.timeline(true));
            p->plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
            p->plot->yAxis->setTicker(QSharedPointer<QCPAxisTickerLog>(new QCPAxisTickerLog));
            p->plot->yAxis->setRange(1,simulateur->nb*0.0012);
        }
        else
        {
            p->plot->graph(0)->setData(simulateur->all,simulateur->nb_mort.timeline());
            p->plot->graph(1)->setData(simulateur->all,simulateur->nb_infecte.timeline());
            p->plot->graph(2)->setData(simulateur->all,simulateur->nb_gueris.timeline());
            p->plot->yAxis->setScaleType(QCPAxis::stLinear);
            p->plot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
            p->plot->yAxis->setRange(0,simulateur->nb*0.0012);
        }
        p->resize_plot(p->Szonetemporelle->value());
        break;

    case GRAPH_CONTAMINATION_JOURNALIER:
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->graph(0)->setPen(QPen(Qt::black));
        p->plot->graph(1)->setPen(QPen(Qt::red));
        p->plot->graph(2)->setPen(QPen(Qt::blue));
        {
            p->plot->graph(0)->setData(simulateur->all_jour,simulateur->dmort.timeline());
            p->plot->graph(1)->setData(simulateur->all_jour,simulateur->dinfecte.timeline());
            p->plot->graph(2)->setData(simulateur->all_jour,simulateur->dgueris.timeline());
            p->plot->yAxis->setScaleType(QCPAxis::stLinear);
            p->plot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
            p->plot->yAxis->setRange(0-simulateur->nb*0.00012,simulateur->nb*0.00012);
        }
        p->plot->xAxis->setRange(0,200);
        break;

    case GRAPH_AGE_CONTAMINATION:
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->addGraph();
        p->plot->graph(0)->setPen(QPen(Qt::green));
        p->plot->graph(1)->setPen(QPen(Qt::black));
        p->plot->graph(2)->setPen(QPen(Qt::red));
        p->plot->graph(0)->setData(simulateur->all,simulateur->moyenne);
        p->plot->graph(1)->setData(simulateur->all,simulateur->moyenne_age_mort.timeline());
        p->plot->graph(2)->setData(simulateur->all,simulateur->moyenne_age_infecte.timeline());
        p->plot->yAxis->setScaleType(QCPAxis::stLinear);
        p->plot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
        p->plot->yAxis->setRange(0,100);
        p->resize_plot(p->Szonetemporelle->value());
        break;
    case GRAPH_HOPITAL:
        p->plot->addGraph();
        p->plot->graph(0)->setPen(QPen(Qt::black));
        p->plot->graph(0)->setData(simulateur->all,simulateur->hopital.timeline());
        p->plot->yAxis->setScaleType(QCPAxis::stLinear);
        p->plot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
        p->plot->yAxis->setRange(0,100);
        p->plot->xAxis->setRange(0,200);
        break;
    }
    p->plot->replot();
    p->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    p->previous_time_data = simulateur->jour*9+simulateur->tranche_actuel;
}

void world::actualise_target_graph(int position)
{
    Graph *p = graphs;
    for(int i = 0;i != position;++i)
        p = (Graph *)p->next;
    if(p->cible->currentIndex() != p->type)
        actualise_graph(position);
}


void world::actualise_soft_graph(int position)
{
    Graph *p = graphs;
    for(int i = 0;i != position;++i)
        p = (Graph *)p->next;
    if(p->previous_time_data != (unsigned long)(simulateur->jour*9+simulateur->tranche_actuel))
    {
        switch(p->type)
        {
        case GRAPH_CONTAMINATION_TOTAL:
            if(p->loga->checkState() == Qt::CheckState::Checked)
            {
                p->plot->graph(0)->setData(simulateur->all,simulateur->nb_mort.timeline(true));
                p->plot->graph(1)->setData(simulateur->all,simulateur->nb_infecte.timeline(true));
                p->plot->graph(2)->setData(simulateur->all,simulateur->nb_gueris.timeline(true));
            }
            else
            {
                p->plot->graph(0)->setData(simulateur->all,simulateur->nb_mort.timeline());
                p->plot->graph(1)->setData(simulateur->all,simulateur->nb_infecte.timeline());
                p->plot->graph(2)->setData(simulateur->all,simulateur->nb_gueris.timeline());
            }
            break;

        case GRAPH_CONTAMINATION_JOURNALIER:
            if(p->loga->checkState() == Qt::CheckState::Checked)
            {
                p->plot->graph(0)->setData(simulateur->all_jour,simulateur->dmort.timeline(true));
                p->plot->graph(1)->setData(simulateur->all_jour,simulateur->dinfecte.timeline(true));
                p->plot->graph(2)->setData(simulateur->all_jour,simulateur->dgueris.timeline(true));
            }
            else
            {
                p->plot->graph(0)->setData(simulateur->all_jour,simulateur->dmort.timeline());
                p->plot->graph(1)->setData(simulateur->all_jour,simulateur->dinfecte.timeline());
                p->plot->graph(2)->setData(simulateur->all_jour,simulateur->dgueris.timeline());
            }
            break;

        case GRAPH_AGE_CONTAMINATION:
            p->plot->graph(0)->setData(simulateur->all,simulateur->moyenne);
            p->plot->graph(1)->setData(simulateur->all,simulateur->moyenne_age_mort.timeline());
            p->plot->graph(2)->setData(simulateur->all,simulateur->moyenne_age_infecte.timeline());
            break;
        case GRAPH_HOPITAL:
            p->plot->graph(0)->setData(simulateur->all,simulateur->hopital.timeline());
            break;
        }
        p->plot->replot();
    }

}

void world::actualise_perso(int position)
{
    Perso *p = personnages;
    for(int i = 0;i != position;++i)
        p = (Perso *)p->next;
    char *blanket = challoc(256);
    sprintf(blanket,"Perso %d",p->index);
    p->fenetre->setWindowTitle(blanket);

    humain *cible = &simulateur->population[p->actual_id];

    sprintf(blanket,"Id : %ld",p->actual_id);
    p->id->setText(blanket);
    p->nom->setText("Nom : "+nom_random(p->actual_id));
    sprintf(blanket,"Age : %d",(int)cible->age);
    p->age->setText(blanket);
    sprintf(blanket,"Chance : %d",255-cible->gravite);
    p->chance->setText(blanket);
    sprintf(blanket,"Maladie : %d",cible->maladie);
    p->maladie->setText(blanket);
    if(cible->dead)
        p->bar->setValue(0);
    else
        p->bar->setValue((int)cible->pv);
    int i;
    for(i = 0;i != cible->nbfamille;++i)
    {
        sprintf(blanket,"%ld",cible->famille[i]);
        p->id_famille[i]->setText(blanket);
    }
    for(;i != 10;++i)
        p->id_famille[i]->setText("");

    int w = p->map->width();
    int h = p->map->height();
    unsigned int black = simulateur->im->pixel(0,0);
    for(i = 0;i != w;++i)
        for(int j = 0;j != h;++j)
        {
            if(simulateur->im->pixel(i*p->factor,j*p->factor) == black)
                p->map->setPixel(i,j,0x49A7FF);
            else
                p->map->setPixel(i,j,0xFF00);
        }

    p->map->setPixel(cible->job_x/p->factor,cible->job_y/p->factor,0x0000FF);
    if(cible->job_x/p->factor != 0)
        p->map->setPixel(cible->job_x/p->factor-1,cible->job_y/p->factor,0x0000FF);
    if(cible->job_x/p->factor != w-1)
        p->map->setPixel(cible->job_x/p->factor+1,cible->job_y/p->factor,0x0000FF);
    if(cible->job_y/p->factor != 0)
        p->map->setPixel(cible->job_x/p->factor,cible->job_y/p->factor-1,0x0000FF);
    if(cible->job_y/p->factor != h-1)
        p->map->setPixel(cible->job_x/p->factor,cible->job_y/p->factor+1,0x0000FF);

    p->map->setPixel(cible->hab_x/p->factor,cible->hab_y/p->factor,0xFF00FF);
    if(cible->hab_x/p->factor != 0)
        p->map->setPixel(cible->hab_x/p->factor-1,cible->hab_y/p->factor,0xFF00FF);
    if(cible->hab_x/p->factor != w-1)
        p->map->setPixel(cible->hab_x/p->factor+1,cible->hab_y/p->factor,0xFF00FF);
    if(cible->hab_y/p->factor != 0)
        p->map->setPixel(cible->hab_x/p->factor,cible->hab_y/p->factor-1,0xFF00FF);
    if(cible->hab_y/p->factor != h-1)
        p->map->setPixel(cible->hab_x/p->factor,cible->hab_y/p->factor+1,0xFF00FF);





    p->supmap->setPixmap(QPixmap::fromImage(*p->map));
    free(blanket);
}

void world::update_need()
{
    need_update = true;
}

void world::real_time()
{
    while(tpsreal->isChecked() && !Pause->isChecked())
    {
        clock_t start = clock();
        silent_tranche();
        clock_t stop = clock();
        qApp->processEvents();
        int time = Delai_input->value()*1000-(((stop-start)*1000)/CLOCKS_PER_SEC);
        if(time > 0)
        {
            Sleep((uint)time);
            qApp->processEvents();
        }
    }
}

void world::extract_graph(int position)
{
    Graph *p = graphs;
    for(int i = 0;i != position;++i)
        p = (Graph *)p->next;
    QMessageBox msgBox;
    msgBox.setText("Veuiller choisir l'extraction");
    msgBox.setInformativeText("");
    QPushButton *pngButton = msgBox.addButton(tr("Extraire une image du graph"), QMessageBox::ActionRole);
    QPushButton *dataButton = msgBox.addButton(tr("Extraire les données relatives"), QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Abort);
    msgBox.exec();
    if(msgBox.clickedButton() == pngButton)
    {
        QString dossier = QFileDialog::getSaveFileName(p,tr("Save File"),SETTINGS->save_file+"graph.png",tr("Images (*.png)"));
        QPixmap im = p->plot->grab();
        im.save(dossier,"PNG");
        compil->add("enrengistrement de "+dossier+" terminé");
    }
    else if(msgBox.clickedButton() == dataButton)
    {
        QString dossier = QFileDialog::getSaveFileName(p,tr("Save File"),SETTINGS->save_file+"graph.txt",tr("Texte (*.txt *.csv)"));
        FILE* file = fopen(dossier.toStdString().c_str(),"w");
        if(file != NULL)
        {
            long date;
            switch(p->type)
            {
            case GRAPH_CONTAMINATION_TOTAL:
                fprintf(file,"Contaminés,Morts,Guéris\n");
                date = simulateur->nb_infecte.date;
                for(long i = 0;i != date;++i)
                    fprintf(file,"%lf,%lf,%lf\n",simulateur->nb_infecte.all[i],simulateur->nb_mort.all[i],simulateur->nb_gueris.all[i]);
                break;
            case GRAPH_CONTAMINATION_JOURNALIER:
                fprintf(file,"Contaminés/j,Morts/j,Guéris/j\n");
                date = simulateur->dinfecte.date;
                for(long i = 0;i != date;++i)
                    fprintf(file,"%lf,%lf,%lf\n",simulateur->dinfecte.all[i],simulateur->dmort.all[i],simulateur->dgueris.all[i]);
                break;
            case GRAPH_AGE_CONTAMINATION:
                fprintf(file,"moyenne_age_infecte,moyenne_age_mort\n");
                date = simulateur->moyenne_age_infecte.date;
                for(long i = 0;i != date;++i)
                    fprintf(file,"%lf,%lf\n",simulateur->moyenne_age_infecte.all[i],simulateur->moyenne_age_mort.all[i]);
                break;
            default:
                fprintf(stdout,"fonctionnalite non implemente ( type = %d )",p->type);
                break;
            }
            fclose(file);
            compil->add("enrengistrement de "+dossier+" terminé");
        }
        else
            compil->add("erreur lors de l'enrengistrement de "+dossier);
    }
}

void world::extract_map(int position)
{
    Mapping *p = map;
    for(int i = 0;i != position;++i)
        p = (Mapping *)p->next;
    QMessageBox msgBox;
    msgBox.setText("Veuiller choisir l'extraction");
    msgBox.setInformativeText("");
    QPushButton *pngButton = msgBox.addButton(tr("Extraire l'image"), QMessageBox::ActionRole);
    QPushButton *dataButton = msgBox.addButton(tr("Extraire les données relatives"), QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Abort);
    msgBox.exec();
    if(msgBox.clickedButton() == pngButton)
    {
        QString dossier = QFileDialog::getSaveFileName(p,tr("Save File"),SETTINGS->save_file+"/map.png",tr("Images (*.png)"));
        p->image->save(dossier,"PNG");
        compil->add("enrengistrement de "+dossier+" terminé");
    }
    else if(msgBox.clickedButton() == dataButton)
    {
        QString dossier = QFileDialog::getSaveFileName(p,tr("Save File"),SETTINGS->save_file+"/map.txt",tr("Texte (*.txt *.csv)"));
        FILE* file = fopen(dossier.toStdString().c_str(),"w");
        if(file != NULL)
        {
            switch(p->type)
            {
            default:
                fprintf(stdout,"fonctionnalite non implemente ( type = %d )",p->type);
                break;
            }
            fclose(file);
            compil->add("enrengistrement de "+dossier+" terminé");
        }
        else
            compil->add("erreur lors de l'enrengistrement de "+dossier);
    }
}

void world::map_clicked(int position)
{
    Mapping *p = map;
    for(int i = 0;i != position;++i)
        p = (Mapping *)p->next;
    QPoint pos = p->supimage->mapFromGlobal(QCursor::pos());

if(p->Szoom->value() >= 1)
        pos *= p->Szoom->value();
    else
        pos /= (1 << (1-p->Szoom->value()));

    char* blanket = challoc(256);
    sprintf(blanket,"Pos : {%d,%d}",pos.x(),pos.y());
    p->info_pos->setText(blanket);
    long index = pos.x()+pos.y()*simulateur->width;



    sprintf(blanket,"Proxis :\n{%llu\n,%llu\n,%llu\n,%llu\n,%llu\n,%llu}"
            ,simulateur->counter_habitation.data[index]
            ,simulateur->counter_densite.data[index]
            ,simulateur->counter_industriel.data[index]
            ,simulateur->counter_loisir.data[index]
            ,simulateur->counter_school.data[index]
            ,simulateur->counter_industriel.data[index]
            );

    p->info_proxi->setText(blanket);


    long r = 0;
    long c = 0;
    for(long i = 0;i != simulateur->nb;++i)
        if(simulateur->population[i].hab_x == pos.x())
            if(simulateur->population[i].hab_y == pos.y())
            {
                r++;
                if(simulateur->population[i].maladie > 0)
                    c++;
            }


    sprintf(blanket,"Population : %ld",r);
    p->info_population->setText(blanket);
    sprintf(blanket,"Contamine : %ld",c);
    p->info_contamination->setText(blanket);
    free(blanket);

}

void world::destroy()
{
    fenetre->hide();
    if(map != NULL)
        map->destroy_all();
    if(graphs != NULL)
        graphs->destroy_all();
    if(personnages != NULL)
        personnages->destroy_all();
    paramesure->fenetre->hide();
    paravirus->fenetre->hide();
    compil->fenetre->hide();
    //delete simulateur;
    delete this;
}

world::~world()
{

}
