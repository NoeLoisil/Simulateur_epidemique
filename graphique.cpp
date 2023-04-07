#include "graphique.h"

Graph::Graph()
{
    fenetre = NULL;
    next = NULL;
    previous = NULL;
    plot = NULL;
    auto_up = NULL;
    update = NULL;
    cible = NULL;
    retract = NULL;
    zonetemporelle = NULL;
    Szonetemporelle = NULL;
    loga = NULL;
    legend = NULL;
    HBL = NULL;
    MAPRIGHT = NULL;
    UP1 = NULL;
    UP2 = NULL;
    UP3 = NULL;
    UP4 = NULL;
    UP5 = NULL;
    retracted = false;
    cache = NULL;
    upactlayout = NULL;
    upact = NULL;
    type = MAP_POPULATION;
    index = 0;
    origin = NULL;
    extract = NULL;
}

Graph::~Graph()
{
    if(fenetre != NULL)
    {
        fenetre->hide();
        delete plot;
        delete auto_up;
        delete update;
        delete cible;
        delete retract;
        delete zonetemporelle;
        delete Szonetemporelle;
        delete loga;
        delete legend;
        delete UP1;
        delete UP2;
        delete UP3;
        delete UP4;
        delete extract;
        delete upact;
        delete upactlayout;
        delete MAPRIGHT;
        delete cache;
        delete fenetre;
    }
}

void Graph::layout(int position,compilateur *comp)
{
    compil = comp;
    index = position;
    retracted = false;
    fenetre = new Fenetre();
    char *gblanket = challoc(32);
    sprintf(gblanket,"Graphique %d",index);
    fenetre->setWindowTitle(gblanket);
    free(gblanket);
    fenetre->show();
    HBL = new QHBoxLayout(fenetre);
    cache = new QWidget();
    MAPRIGHT = new QVBoxLayout(cache);
    plot = new QCustomPlot();
    plot->addGraph();
    plot->replot();
    HBL->addWidget(plot,1);
    HBL->addWidget(cache);
    auto_up = new QCheckBox();
    auto_up->setText("Auto-update");
    update = new QPushButton();
    update->setText("Update");
    cible = new QComboBox();
    cible->addItem("Contamination total");
    cible->addItem("Contamination journalière");
    cible->addItem("Victime /âge");
    cible->addItem("Hopital");
    type = GRAPH_CONTAMINATION_TOTAL;
    cible->setCurrentIndex(0);
    connect(cible,SIGNAL(currentIndexChanged(int)),this,SLOT(Switchtype(int)));
    retract = new QPushButton();
    upact = new QPushButton();
    upactlayout = new QVBoxLayout();
    upactlayout->addWidget(upact);
    upactlayout->addStretch();
    retract->setText("<<");
    upact->setText(">>");
    zonetemporelle = NULL;
    Szonetemporelle = NULL;
    loga = NULL;
    zonetemporelle = new QLabel();
    zonetemporelle->setText("Zone temporelle :");
    loga = new QCheckBox();
    loga->setText("Echelle logarithmique");
    connect(loga,SIGNAL(clicked()),this,SLOT(Logapressed()));
    Szonetemporelle = new QSlider();
    Szonetemporelle->setOrientation(Qt::Orientation::Horizontal);
    Szonetemporelle->setRange(16,44);
    old_time = 8;
    Szonetemporelle->setValue(8);
    legend = new QGroupBox();
    extract = new QPushButton();
    UP1 = new QHBoxLayout();
    UP2 = new QHBoxLayout();
    UP3 = new QHBoxLayout();
    UP4 = new QHBoxLayout();
    UP5 = new QHBoxLayout();
    MAPRIGHT->addLayout(UP1);
    MAPRIGHT->addLayout(UP2);
    MAPRIGHT->addLayout(UP3);
    MAPRIGHT->addLayout(UP4);
    MAPRIGHT->addWidget(Szonetemporelle);
    MAPRIGHT->addLayout(UP5);
    MAPRIGHT->addWidget(legend,1);
    MAPRIGHT->addWidget(extract);
    UP1->addWidget(auto_up);
    UP1->addWidget(retract);
    UP2->addWidget(update);
    UP2->addStretch();
    UP3->addWidget(cible);
    UP3->addStretch();
    UP4->addWidget(zonetemporelle);
    UP4->addStretch();
    UP5->addWidget(loga);
    UP5->addStretch();
    legend->showMaximized();
    legend->setTitle("Légende :");
    extract->setText("extraire");
    connect(extract,SIGNAL(clicked()),this,SLOT(need_extract()));
    HBL->addLayout(upactlayout);
    upact->hide();
    upact->setFixedSize(31,23);
    retract->setFixedSize(31,23);
    connect(retract,SIGNAL(clicked()),this,SLOT(Switchmode()));
    connect(upact,SIGNAL(clicked()),this,SLOT(Switchmode()));
    connect(update,SIGNAL(clicked()),this,SLOT(Updatepressed()));
    connect(fenetre,SIGNAL(closing()),this,SLOT(destroy()));
    connect(Szonetemporelle,SIGNAL(valueChanged(int)),this,SLOT(update_time(int)));
}

void Graph::Logapressed()
{
    if(auto_up->checkState() == Qt::CheckState::Checked)
        emit actualise(index);
}

void Graph::Switchmode()
{
    if (retracted)
    {
        cache->show();
        upact->hide();
    }
    else
    {
        cache->hide();
        upact->show();
    }
    retracted = !retracted;
}
void Graph::Switchtype(int newtype)
{
    switch(newtype)
    {
    case GRAPH_CONTAMINATION_TOTAL:
        loga->setEnabled(true);
    break;
    default:
        loga->setEnabled(false);
        loga->setCheckState(Qt::CheckState::Unchecked);
    break;
    }

    emit actualise_target(index);
}

void Graph::Updatepressed()
{
    emit actualise(index);
}

void Graph::need_extract()
{
    emit extraction(index);
}

void Graph::destroy()
{
    compil->add("destroy graph num ",index);
    if(previous != NULL)
        ((Graph *)previous)->next = next;
    else
        *origin = next;
    if(next != NULL)
        ((Graph *)next)->previous = previous;

    Graph *p = (Graph *)next;
    while(p != NULL)
    {
        p->index--;
        p = (Graph *)p->next;
    }
    delete this;
}

#define GRAPH_FACTOR_ZOOM_X 4.

void Graph::update_time(int newindex)
{
    plot->xAxis->scaleRange(exp((newindex-old_time)/GRAPH_FACTOR_ZOOM_X));
    old_time = newindex;
    plot->replot();
}

void Graph::resize_plot(int new_time)
{
    old_time = new_time;
    switch(type)
    {
        case GRAPH_CONTAMINATION_TOTAL:case GRAPH_AGE_CONTAMINATION:
        plot->xAxis->setRange(0,exp(new_time/GRAPH_FACTOR_ZOOM_X));
        break;
        default:
        plot->xAxis->setRange(0,exp(new_time/(GRAPH_FACTOR_ZOOM_X*9)));
        break;
    }


}

void Graph::destroy_all()
{
    if(next != NULL)
        ((Graph *)next)->destroy_all();
    delete this;
}
