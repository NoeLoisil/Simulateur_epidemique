#include "map.h"

Mapping::Mapping()
{
    fenetre = NULL;
    next = NULL;
    previous = NULL;
    image = NULL;
    supimage = NULL;
    Vscrbar = NULL;
    Hscrbar = NULL;
    auto_up = NULL;
    update = NULL;
    cible = NULL;
    retract = NULL;
    zoom = NULL;
    seuil = NULL;
    Szoom = NULL;
    Sseuil = NULL;
    legend = NULL;
    HBL = NULL;
    MAPSCR = NULL;
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

Mapping::~Mapping()
{
    if(fenetre != NULL)
    {
        fenetre->hide();
        delete image;
        delete supimage;
        delete Vscrbar;
        delete Hscrbar;
        delete auto_up;
        delete update;
        delete cible;
        delete retract;
        delete zoom;
        delete seuil;
        delete Szoom;
        delete Sseuil;
        delete info_pos;
        delete info_population;
        delete info_proxi;
        delete info_contamination;
        delete legend;
        delete extract;
        delete UP1;
        delete UP2;
        delete UP3;
        delete UP4;
        delete upact;
        delete upactlayout;
        delete MAPSCR;
        delete MAPRIGHT;
        delete cache;
        delete HBL;
        delete fenetre;
    }
}

void Mapping::layout(int position,compilateur *comp,const QSize size)
{
    compil = comp;
    index = position;
    retracted = false;
    fenetre = new Fenetre();
    char *blanket = challoc(16);
    sprintf(blanket,"Map %d",index);
    fenetre->setWindowTitle(blanket);
    free(blanket);
    fenetre->show();
    HBL = new QHBoxLayout(fenetre);
    MAPSCR = new QGridLayout();
    cache = new QWidget();
    MAPRIGHT = new QVBoxLayout(cache);
    image = new QImage(size,QImage::Format::Format_RGB32);
    supimage = new ClickableLabel();
    HBL->addLayout(MAPSCR,1);

    MAPSCR->addWidget(supimage,0,0);
    supimage->setPixmap(QPixmap::fromImage(*image));
    Vscrbar = new QScrollBar();
    Vscrbar->setOrientation(Qt::Orientation::Vertical);
    MAPSCR->addWidget(Vscrbar,0,1);
    HBL->addWidget(cache);
    Hscrbar = new QScrollBar();
    Hscrbar->setOrientation(Qt::Orientation::Horizontal);
    MAPSCR->addWidget(Hscrbar,1,0);
    auto_up = new QCheckBox();
    auto_up->setText("Auto-update");
    update = new QPushButton();
    update->setText("Update");
    cible = new QComboBox();
    cible->addItem("Population");
    cible->addItem("Contamination absolue");
    cible->addItem("Contamination relative");
    cible->addItem("Morts");
    cible->addItem("Proxi");
    connect(cible,SIGNAL(currentIndexChanged(int)),this,SLOT(Switchtype(int)));
    retract = new QPushButton();
    upact = new QPushButton();
    upactlayout = new QVBoxLayout();
    upactlayout->addWidget(upact);
    upactlayout->addStretch();
    retract->setText("<<");
    upact->setText(">>");
    zoom = new QLabel();
    zoom->setText("Zoom :");
    seuil = new QLabel();
    seuil->setText("Seuil :");
    Szoom = new QSlider();
    Sseuil = new QSlider();
    Sseuil->setRange(0,10);
    Sseuil->setValue(3);
    Szoom->setOrientation(Qt::Orientation::Horizontal);
    Szoom->setRange(-1,max(min(880/size.height(),1720/size.width()),1));
    Szoom->setValue(1);
    connect(Sseuil,SIGNAL(valueChanged(int)),this,SLOT(Updatezoom(int)));
    connect(Szoom,SIGNAL(valueChanged(int)),this,SLOT(Updatezoom(int)));
    Sseuil->setOrientation(Qt::Orientation::Horizontal);
    legend = new QGroupBox();
    info_pos = new QLabel(legend);
    info_population = new QLabel(legend);
    info_proxi = new QLabel(legend);
    info_contamination = new QLabel(legend);
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
    MAPRIGHT->addWidget(Szoom);
    MAPRIGHT->addLayout(UP5);
    MAPRIGHT->addWidget(Sseuil);
    MAPRIGHT->addWidget(legend,1);
    MAPRIGHT->addWidget(extract);
    UP1->addWidget(auto_up);
    UP1->addWidget(retract);
    UP2->addWidget(update);
    UP2->addStretch();
    UP3->addWidget(cible);
    UP3->addStretch();
    UP4->addWidget(zoom);
    UP4->addStretch();
    UP5->addWidget(seuil);
    UP5->addStretch();
    legend->showMaximized();
    legend->setTitle("Légende :");
    info_pos->setGeometry(10,10,100,20);
    info_population->setGeometry(10,30,100,20);
    info_proxi->setGeometry(10,50,100,100);
    info_contamination->setGeometry(10,150,100,20);
    info_pos->setText("Pos : {0,0}");
    info_population->setText("Population : 0");
    info_proxi->setText("Proxis :\n{0\n,0\n,0\n,0\n,0\n,0}");
    info_contamination->setText("Contamine : 0");
    extract->setText("extraire");
    HBL->addLayout(upactlayout);
    upact->hide();
    upact->setFixedSize(31,23);
    retract->setFixedSize(31,23);
    connect(extract,SIGNAL(clicked()),this,SLOT(need_extract()));
    connect(retract,SIGNAL(clicked()),this,SLOT(Switchmode()));
    connect(upact,SIGNAL(clicked()),this,SLOT(Switchmode()));
    connect(update,SIGNAL(clicked()),this,SLOT(Updatepressed()));
    connect(fenetre,SIGNAL(closing()),this,SLOT(destroy()));
    connect(supimage,SIGNAL(clicked()),this,SLOT(map_pressed()));
}

void Mapping::map_pressed()
{
    emit clicked(index);
}

void Mapping::Switchmode()
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
void Mapping::Switchtype(int newtype)
{
    type = newtype;
    if(auto_up->isChecked())
        emit actualise(index);
}

void Mapping::Updatepressed()
{
    emit actualise(index);
}

void Mapping::Updatezoom(int newzoom)
{
    if(auto_up->isChecked())
        emit actualise(index);
}

void Mapping::need_extract()
{
    emit extraction(index);
}

void Mapping::destroy()
{
    compil->add("destroy map num ",index);
    if(previous != NULL)
        ((Mapping *)previous)->next = next;
    else
        *origin = next;
    if(next != NULL)
        ((Mapping *)next)->previous = previous;

    Mapping *p = (Mapping *)next;
    while(p != NULL)
    {
        p->index--;
        p = (Mapping *)p->next;
    }
    delete this;
}

void Mapping::destroy_all()
{
    if(next != NULL)
        ((Mapping *)next)->destroy_all();
    delete this;
}
