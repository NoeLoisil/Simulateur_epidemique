#include "simulateur.h"


#define COMPIL_FAMILY false
#define COMPIL_HOUSE false
char *simblanket = challoc(1024);


data::data(bool has_incr,unsigned int factor) : all(data_size_all/factor)
{
    if(has_incr)
        all_incr = new QVector<double>(data_size_all/factor);
    else
        all_incr = NULL;
   increment = has_incr;
   date = 0;
}

double Simulateur::degat(int gravite,int tranche)
{
    double a =sqrt(log((gravite+21.)/20.)*gravite*sqrt(gravite))+91.;
    return facteur_degat*sqrt(0.0125*gravite*exp(-(13.*tranche/9.-a)*(13.*tranche/9.-a)/((gravite+1)*sqrt(gravite+1))))*0.85;
}

void data::reset(long value)
{
    all.fill(value);
    if(increment)
        all_incr->fill(max(value+1,1));
    date = 0;
}

void data::add(long value)
{
    all[date] = value;
    if(increment)
        (*all_incr)[date] = max(value+1,1);
    date++;
}

void data::add(double value)
{
    all[date] = value;
    if(increment)
        (*all_incr)[date] = max(value+1.,1.);
    date++;
}

QVector<double> data::timeline(bool incr)
{
    if(incr && increment)
        return *all_incr;
    return all;
}



bool enfantdispo = true;


Simulateur::Simulateur(FILE *file,compilateur *c,loading_bar *load,QString dossier,long seed,char *path_map) :
    hopital(false),nb_mort(true),nb_sains(),nb_infecte(true),nb_gueris(true),
    dmort(false,9),dinfecte(false,9),dgueris(false,9),
    moyenne_age_infecte(false),moyenne_age_mort(false),moyenne(data_size_all),
    all(data_size_all),all_jour(data_size_all_jour)
{
    compil = c;
    loading = load;
    nb = 0;
    population = NULL;
    distanciel = NULL;
    tranche_actuel = 0;
    jour = 0;
    im = NULL;
    for(long i = 0;i != data_size_all;++i)
        all[i] = i/9.;
    for(long i = 0;i != data_size_all_jour;++i)
        all_jour[i] = i;
    srand(seed);
    im = new QImage();
    im->load(dossier+"/"+path_map);
    width = im->size().width();
    height = im->size().height();
    imsize = width*height;
    counter_habitation.alloc(width,height);
    counter_industriel.alloc(width,height);
    counter_commerce.alloc(width,height);
    counter_loisir.alloc(width,height);
    counter_school.alloc(width,height);
    counter_densite.alloc(width,height);
    inverse_densite = new double[width*height];
    counter_habitation.reset();
    counter_industriel.reset();
    counter_commerce.reset();
    counter_loisir.reset();
    counter_school.reset();
    counter_densite.reset();

    tranche_actuel = 0;
    jour = 0;
    nb = take_long(file);
    sprintf(simblanket,"Creation d'une population de %ld individus...\n",nb);
    compil->add(simblanket);
    population = new humain[nb];
    for(long i = 0;i != nb;++i)
    {
        population[i].hab_x = -1;
        population[i].hab_y = -1;
        population[i].job_x = -1;
        population[i].job_y = -1;
        population[i].famille = NULL;
        population[i].dead = false;
        population[i].nbfamille = 0;
        population[i].maladie = 0;
        population[i].pv = 100.;
        population[i].hopital = false;
    }
    act_hop = 0;
    if(population == NULL)
        compil->add("erreur fatal 000");
    if(nb*sizeof(humain) >= 2000000)
        sprintf(simblanket,"Allocation de %lu Mo pour la population réussi\n",nb*sizeof(humain)/1000000);
    else
        sprintf(simblanket,"Allocation de %lu Ko pour la population réussi\n",nb*sizeof(humain)/1000);
    compil->add(simblanket);
    setup_age(file);
    setup_family(file);
    setup_geo(file);

    hopital.reset(0);
    nb_mort.reset(0);
    nb_gueris.reset(0);
    nb_sains.reset(nb);
    nb_infecte.reset(0);
    dmort.reset(0);
    dgueris.reset(0);
    dinfecte.reset(0);
    moyenne_age = 0;
    moyenne_age = getmoyenneage();
    moyenne_age_infecte.reset(moyenne_age);
    moyenne_age_mort.reset(moyenne_age);
    moyenne.fill(moyenne_age);

    mal_prop = take_long(file);


    compil->add("Mémoire utilisé par la simulation : ",(int)memory_taked());
}

Simulateur::~Simulateur()
{
    for(long i = 0;i != nb;++i)
        delete[] population[i].famille;
    delete[] population;
    delete im;
}




void Simulateur::setup_age(FILE* file)
{
    compil->add("repartition des ages...\n");
    unsigned long nb_age  = take_long(file);
    int *values = new int[3*nb_age];
    for(unsigned long i = 0;i != nb_age;++i)
        fscanf(file,"%d-%d = %d\n",&values[i*3+1],&values[i*3+2],&values[i*3]);
    double ftpm = 0;
    for(unsigned long i = 0;i != nb_age;++i)
        ftpm += values[3*i];
    long tpm = 0;
    long tpm3 = 0;
    for(unsigned long i = 0;i != nb_age-1;++i)
    {
        tpm3 += values[3*i];
        long tpm2 = nb*tpm3/ftpm;
        double ftpm2 = values[i*3+1];
        if(tpm2 != tpm)
        {
            double ftpm3 = ((float)values[i*3+2]-(float)values[i*3+1])/((float)tpm2-(float)tpm);
            for(long j = tpm;j != tpm2;++j)
            {
                population[j].age = ftpm2;
                ftpm2 += ftpm3;
            }
            tpm = tpm2;
        }
    }
    double ftpm2 = values[nb_age*3-2];
    double ftpm3 = ((float)values[nb_age*3-1]-(float)values[nb_age*3-2])/((float)nb-(float)tpm);
    for(long i = tpm;i != nb;++i)
    {
        population[i].age = ftpm2;
        ftpm2 += ftpm3;
    }
    for(long i = 0;i != nb;++i)
    {
        double chance = ((double)(rand32()%1000))/1000.;
        population[i].gravite = 1.15*(sqrt(population[i].age)*10+10)*(0.8+2*(chance*chance-0.4));
    }
    compil->add("repartition des ages termines\n");
}
long enfant_seul;
void Simulateur::setup_family(FILE* file)
{
    compil->add("repartition familial...\n");
    long seul = take_long(file),sansenfant = take_long(file),monoparental = take_long(file),increment = take_long(file),stop = take_long(file);
    int previous_seul = -1,previous_couple = -1;
    nbenfant = getnbendessous(20);
    enfant_seul = nbenfant;
    for(long i = nbenfant;i != nb;++i)
    {
        if(rand() > RAND_MAX/100*seul)
        {
            if(rand() < RAND_MAX/100*sansenfant || population[i].age > 65. || !enfantdispo)
            {
                if(previous_seul == -1)
                    previous_seul = i;
                else
                {
                    population[i].famille = new long[1];
                    population[previous_seul].famille = new long[1];
                    population[i].nbfamille = 1;
                    population[previous_seul].nbfamille = 1;
                    population[i].famille[0] = previous_seul;
                    population[previous_seul].famille[0] = i;
#if COMPIL_FAMILY
                    compil->add("Création d'un couple seul :",previous_seul,i);
#endif
                    previous_seul = -1;
                }
            }
            else
            {
                if(rand() < RAND_MAX/100*monoparental)
                {
                    int *child = new int[stop];
                    for(long j = 0;j != stop;++j)child[j] = -1;
                    long j = 0;double ftpm = 1.;
                    while((double)rand() < (double)RAND_MAX*ftpm && j != stop && enfantdispo)
                    {
                        child[j] = adopte();
                        if(child[j] != -1)
                        {
                            population[child[j]].nbfamille = 1;
                            ftpm *= ((double)increment)/100.;
                            j++;
                        }
                    }
                    long tpm = 0;
                    for(j = 0;j != stop;++j)
                        if(child[j] != -1)
                            tpm++;
                    population[i].nbfamille = tpm;
                    population[i].famille = new long[tpm];
                    for(j = 0;j != tpm;++j)
                        population[i].famille[j] = child[j];
                    for(j = 0;j != tpm;++j)
                    {
                        population[child[j]].nbfamille = tpm;
                        population[child[j]].famille = new long[tpm];
                        population[child[j]].famille[0] = i;
                        long l = 1;
                        for(long k = 0;k != tpm;k++)
                            if(k != j)
                                population[child[j]].famille[l++] = child[k];
                    }
#if COMPIL_FAMILY
                    compil->add("Création d'une famille monoparental :",(int)i,child,(long)tpm);
#endif
                    delete[] child;
                }
                else
                {
                    if(previous_couple == -1)
                        previous_couple = i;
                    else
                    {
                        int *child = new int[stop];
                        for(long j = 0;j != stop;++j)child[j] = -1;
                        long j = 0;double ftpm = 1;
                        while(rand() < RAND_MAX*ftpm && j != stop && enfantdispo)
                        {
                            child[j] = adopte();
                            if(child[j] != -1)
                            {
                                population[child[j]].nbfamille = 1;
                                ftpm *= ((double)increment)/100.;
                                j++;
                            }
                        }
                        long tpm = 1;
                        for(j = 0;j != stop;++j)
                            if(child[j] != -1)
                                tpm++;
                        population[i].nbfamille = tpm;
                        population[i].famille = new long[tpm];
                        population[i].famille[0] = previous_couple;
                        population[previous_couple].nbfamille = tpm;
                        population[previous_couple].famille = new long[tpm];
                        population[previous_couple].famille[0] = i;
                        for(j = 0;j != tpm-1;++j)
                        {
                            population[i].famille[j+1] = child[j];
                            population[previous_couple].famille[j+1] = child[j];
                        }
                        for(j = 0;j != tpm-1;++j)
                        {
                            population[child[j]].nbfamille = tpm;
                            population[child[j]].famille = new long[tpm];
                            population[child[j]].famille[0] = i;
                            population[child[j]].famille[1] = previous_couple;
                            long l = 2;

                            for(long k = 0;k != tpm-1;k++)
                                if(k != j)
                                {
                                    population[child[j]].famille[l] = child[k];
                                    l++;
                                }
                        }
#if COMPIL_FAMILY
                        compil->add("Création d'une famille :",(int)previous_couple,(int)i,child,(long)tpm-1);
#endif
                        delete[] child;
                        previous_couple = -1;
                    }
                }
            }
        }
#if COMPIL_FAMILY
        else
            compil->add("Création d'un solitaire:",i);
#endif

    }
    long j = 0;
    for(long i = 0;i != nbenfant-1;++i)
        if(population[i].nbfamille == 0) j++;

    compil->add("Orphelins : ",(int)j);
    compil->add("repartition familliale terminé\n");
}

void Simulateur::setup_geo(FILE* file)
{
    compil->add("repartition geographique...\n");
    long luck_job = nb/max(width,height);
    long nb_start = take_long(file);
    Uint64 total = 0;
    int h = im->height();
    int w = im->width();
    int s = h*w;
    Uint8 *red = new Uint8[s];
    Uint8 *blue = new Uint8[s];
    compil->add("dimension de la map ",w,h);
    long k = 0;
    for(int i = 0;i != h;++i)
        for(int j = 0;j != w;++j)
        {
            red[k] = im->pixelColor(j,i).red();
            blue[k] = im->pixelColor(j,i).blue();
            k++;
        }
    Uint64* refh = new Uint64[h+1];
    for(int i = 0;i != h;++i)
    {
        refh[i] = total;
        for(long j = 0;j != w;++j)
            total += red[j+i*w];
    }
    compil->add("total poids habitation : ",(int)total);
    refh[h] = total;

    humain *cible = &population[0];
    humain *last_human = cible + nb;
    for(humain *cible = &population[0];cible != last_human;++cible)
    {
        if(cible->hab_x == -1)
        {
            Uint64 location = (rand64())%total;
            Uint32 pos_y = 0;
            while(refh[pos_y] < location) pos_y++;pos_y--;
            Uint32 pos_x = 0;Uint64 actual = refh[pos_y];
            while(actual < location)
                actual += red[(pos_x++ + w*pos_y)%s];
            cible->hab_x = pos_x;
            cible->hab_y = pos_y;

            if(rand32()%luck_job == 0 && cible->age >= 20)
            {
                Uint64 location_job = (rand64())%total;
                Uint32 job_y = 0;
                while(refh[job_y] < location_job) job_y++;job_y--;
                Uint32 job_x = 0;Uint64 actual_job = refh[job_y];
                while(actual_job < location_job)
                    actual_job += red[(job_x++ + w*job_y)%s];


                cible->job_x = job_x;
                cible->job_y = job_y;
            }
            else
            {
                cible->job_x = pos_x;
                cible->job_y = pos_y;
            }

#if COMPIL_HOUSE
            compil->add("hebergement ",i,pos_x,pos_y);
#endif
            for(int j = 0;j != cible->nbfamille;++j)
            {
                population[cible->famille[j]].hab_x = pos_x;
                population[cible->famille[j]].hab_y = pos_y;
                if(rand32()%luck_job == 0 && population[cible->famille[j]].age >= 20)
                {
                    Uint64 location_job = (rand64())%total;
                    Uint32 job_y = 0;
                    while(refh[job_y] < location_job) job_y++;job_y--;
                    Uint32 job_x = 0;Uint64 actual_job = refh[job_y];
                    while(actual_job < location_job)
                        actual_job += red[(job_x++ + w*job_y)%s];


                    population[cible->famille[j]].job_x = job_x;
                    population[cible->famille[j]].job_y = job_y;
                }
                else
                {
                    population[cible->famille[j]].job_x = pos_x;
                    population[cible->famille[j]].job_y = pos_y;
                }
#if COMPIL_HOUSE
                compil->add("hebergement ",population[i].famille[j],pos_x,pos_y);
#endif
            }
        }
    }
    delete[] refh;

    long tot = 0;
    for(humain *cible = &population[0];cible != last_human;++cible)
    {
        if(blue[cible->hab_x+cible->hab_y*width])
            tot++;
    }
    if(tot != 0)
    {
        nb_start = min(tot,nb_start);
        long *list = new long[tot];
        long index = 0;
        for(long i = 0;i != nb;++i)
        {
            if(blue[population[i].hab_x+population[i].hab_y*width])
                list[index++] = i;
        }
        for(long i = 0;i != nb_start;++i)
        {
            population[list[i*tot/nb_start]].maladie = 1;
        }
        delete[] list;
    }

    compil->add("repartition géographique terminée\n");
}

#define PARTITION 8


long partition_s[PARTITION] = {0,0,0,0,0,0,0,0};

long Simulateur::adopte()
{
    if(enfantdispo || true)
    {
        long id = rand()%PARTITION;
        long tpm = id*(nbenfant-1)/PARTITION+partition_s[id];
        while(tpm != nbenfant-1)
        {
            if(population[tpm].nbfamille == 0)
            {
                partition_s[id] = tpm-id*(nbenfant-1)/PARTITION;
                return tpm;
            }
            tpm++;
        }
        tpm = partition_s[0];
        while(tpm != nbenfant-1)
        {
            if(population[tpm].nbfamille == 0)
            {
                partition_s[0] = tpm;
                return tpm;
            }
            tpm++;
        }
    }
    enfantdispo = false;
    return -1;
}

long Simulateur::getnb(int age)
{
    if(age > 100) return 0;
    if(age < 0) return 0;
    long index = 0;
    while(int(population[index].age) != age && index != nb) index++;
    unsigned long result = 0;
    while(int(population[index].age) == age && index != nb)
    {
        if(!population[index].dead) ++result;
        ++index;
    }
    return result;
}

long Simulateur::getnbaudessus(int age)
{
    if(age >= 100) return 0;
    if(age < 0) return nb;
    long index = 0;
    long i = 0;
    while(int(population[i].age) <= age && index != nb) i++;
    unsigned long result = 0;
    while(index != nb)
    {
        if(!population[index].dead) ++result;
        ++index;
    }
    return result;
}

long Simulateur::getnbendessous(int age)
{
    if(age >= 100) return nb;
    if(age < 0) return 0;
    long index = 0, result = 0;
    while(int(population[index].age) < age && index != nb)
    {
        if(!population[index].dead) ++result;
        ++index;
    }
    return result;
}

double Simulateur::getmoyenneagemort()
{
    double result = 0;
    unsigned long nbmorts = 0;
    for(long i = 0;i != nb;++i)
        if(population[i].dead)
        {
            nbmorts++;
            result += population[i].age;
        }
    if(nbmorts == 0) return moyenne_age;
    return result/nbmorts;
}

double Simulateur::getmoyenneageinfecte()
{
    double result = 0;
    unsigned long nbinfecte = 0;
    for(long i = 0;i != nb;++i)
        if(population[i].maladie > 0)
        {
            nbinfecte++;
            result += population[i].age;
        }
    if(nbinfecte == 0) return moyenne_age;
    return result/nbinfecte;
}

double Simulateur::getmoyenneage()
{
    double result = 0;
    unsigned long nbvivant = 0;
    for(long i = 0;i != nb;++i)
        if(!population[i].dead)
        {
            nbvivant++;
            result += population[i].age;
        }
    if(nbvivant == 0) return moyenne_age;
    return result/nbvivant;
}

void Simulateur::tranches(unsigned long n)
{
    loading->reset(n,"calcul en cours");
    for(unsigned long i = 0;i != n;++i)
    {
        tranche();
        loading->actualise(i+1);
    }
    loading->end();
}

long Simulateur::getnbdeath()
{
    long r = 0;
    for(long i = 0;i != nb;++i)
        if(population[i].dead) ++r;
    return r;
}

long Simulateur::getnbsains()
{
    long r = 0;
    for(long i = 0;i != nb;++i)
        if(!population[i].dead && population[i].maladie == 0) ++r;
    return r;
}

long Simulateur::getnbgueris()
{
    long r = 0;
    for(long i = 0;i != nb;++i)
        if(!population[i].dead && population[i].maladie == -1) ++r;
    return r;
}

long Simulateur::getnbinfecte()
{
    long r = 0;
    for(long i = 0;i != nb;++i)
        if(!population[i].dead && population[i].maladie > 0) ++r;
    return r;
}

void counter::reset()
{
    std::fill(data,data+w*h,0);
}

void counter::add(int x,int y,int factor,int distance_zone,Uint64 *distanciel)
{
    Sint16 x_s = x - distance_zone,y_s = y - distance_zone,x_e = x + distance_zone + 1,y_e = y + distance_zone + 1;
    x_s = max(0,x_s);y_s = max(0,y_s),x_e = min(w,x_e),y_e = min(h,y_e);
    for(Sint16 iy = y_s;iy != y_e;++iy)
    {
        Uint32 index = iy*w+x_s;
        for(Sint16 ix = x_s;ix != x_e;++ix)
            data[index++] += factor*distanciel[abs(ix-x)+abs(iy-y)];
    }
    /*
    char *blanket;
    blanket = challoc(128);
    sprintf(blanket,"%llu",distanciel[0]);
    qDebug(blanket);

    free(blanket);
    */
}

void counter::alloc(int w,int h)
{
    this->w =w;
    this->h = h;
    data = new Uint64[w*h];
}

Uint64 counter::get(int x,int y)
{
    return data[x+y*w];
}


void Simulateur::addcontamine(humain *cible,int factor)
{
    if(cible->age < 20)
    {
        counter_school.add(cible->hab_x,cible->hab_y,factor,distance_zone,distanciel);
        counter_habitation.add(cible->hab_x,cible->hab_y,factor,distance_zone,distanciel);
    }
    else
    {
        counter_commerce.add(cible->hab_x,cible->hab_y,factor,distance_zone,distanciel);
        counter_industriel.add(cible->job_x,cible->job_y,factor,distance_zone,distanciel);
        counter_habitation.add(cible->hab_x,cible->hab_y,factor,distance_zone,distanciel);
        counter_loisir.add(cible->hab_x,cible->hab_y,factor,distance_zone,distanciel);
    }
}

double fd(pourcentage distanciation)
{
    return 100.-distanciation*distanciation*0.009;
}

double conta_prob(double x)
{
    return x*(0.125278+x*(-0.536389+x*(0.872222+x*-0.361111)));
}

#define SOIN_HOPITAL 0.5
void Simulateur::tranche()
{

    double cste = 0.00000000000015*(10000-nbmasque*effmasque)*notautoconfinement*efficacite_zone*fd(distanciation);

    int start_infect = temps_infection-debut_infecteux;
    int end_infect = temps_infection-debut_infecteux-temps_infecteux;
    if(end_infect < 0 && start_infect >= 0)
        end_infect = 0;
    double f_infection;
    if(temps_infection == 189)
        f_infection = 1.;
    else
        f_infection = 189./temps_infection;

    humain *cible = &population[0];
    humain *last_human = cible + nb;

    double ecole_cste = cste*ecole.ouverture[tranche_actuel]*0.01;
    double commerce_cste = cste*commerce.ouverture[tranche_actuel]*0.01;
    double industriel_cste = cste*industriel.ouverture[tranche_actuel]*0.0001*(100-teletravail);
    double loisir_cste = cste*loisir.ouverture[tranche_actuel]*0.01;

    for(long i = 0;i != vaccinationjournaliere;++i)
    {
        population[rand32()%nb].maladie = -1;
    }

    for(long i = 0;i != nouveau;++i)
    {
        long index = rand32()%nb;
        if(population[index].maladie == 0)
            population[index].maladie = temps_infection;
    }


    for(humain *cible = &population[0];cible != last_human;++cible)
    {
            if(cible->maladie > 0)
            {
                cible->maladie -= 1;
                if(cible->pv < 30)
                {
                    if(cible->hopital)
                        cible->pv -= precalcdegat[int((temps_infection-cible->maladie)*f_infection+cible->gravite*189+0.5)]-SOIN_HOPITAL;
                    else
                    {
                        if(cible->pv <= 20)
                        {
                            if(act_hop != hopital_re)
                            {
                                act_hop++;
                                cible->hopital = true;
                                cible->pv -= precalcdegat[int((temps_infection-cible->maladie)*f_infection+cible->gravite*189+0.5)]-SOIN_HOPITAL;
                            }
                            else
                                cible->pv -= precalcdegat[int((temps_infection-cible->maladie)*f_infection+cible->gravite*189+0.5)];
                        }
                        else
                            cible->pv -= precalcdegat[int((temps_infection-cible->maladie)*f_infection+cible->gravite*189+0.5)];
                    }
                }
                else
                    cible->pv -= precalcdegat[int((temps_infection-cible->maladie)*f_infection+cible->gravite*189+0.5)];

                if(cible->pv <= 0)
                {
                    cible->dead = true;
                    if(cible->maladie >= end_infect && cible->maladie < start_infect)
                    {
                        addcontamine(cible,-1);
                        // count -1
                    }
                    cible->maladie = -1;
                    if(cible->hopital)
                    {
                        cible->hopital = false;
                        act_hop--;
                    }
                }
                else
                {
                    if(cible->maladie == start_infect) // 126
                    {
                        addcontamine(cible,1);
                        // count +1
                    }
                    if(cible->maladie == end_infect) // 63
                    {
                        addcontamine(cible,-1);
                    }
                    if(cible->maladie == 0)
                    {
                        cible->maladie = -1;
                        if(cible->hopital)
                        {
                            cible->hopital = false;
                            act_hop--;
                        }
                    }
                }
                if(cible->maladie >= end_infect && cible->maladie < start_infect)
                {
                    int n = cible->nbfamille;
                    for(int i = 0;i != n;++i)
                    {
                        if(population[cible->famille[i]].maladie == 0)
                        {
                            if(alea32(0.000002*(efficacite_famille*(notautoconfinement*0.8))))
                                population[cible->famille[i]].maladie = temps_infection;
                        }
                    }
                }
            }
            else if(cible->maladie == 0)
            {
                if(!cible->dead)
                {
                    long posindexhab = width*cible->hab_y+cible->hab_x;
                    if(counter_habitation.data[posindexhab] != 0)
                    {
                        if(alea32(cste*conta_prob((double)counter_habitation.data[posindexhab]*inverse_densite[posindexhab])))
                            cible->maladie = temps_infection;
                        else if(loisir_cste*alea32(cste*conta_prob((double)counter_loisir.data[posindexhab]*inverse_densite[posindexhab])))
                            cible->maladie = temps_infection;
                        else if(cible->age < 20)
                        {
                            if(alea32(ecole_cste*conta_prob((double)counter_school.data[posindexhab]*inverse_densite[posindexhab])))
                                cible->maladie = temps_infection;
                        }
                        else
                        {
                            if(alea32(commerce_cste*conta_prob((double)counter_commerce.data[posindexhab]*inverse_densite[posindexhab])))
                                cible->maladie = temps_infection;
                        }
                    }
                    if(counter_industriel.data[posindexhab] != 0)
                    {
                        if(cible->age >= 20)
                            if(alea32(industriel_cste*conta_prob((double)counter_industriel.data[posindexhab]*inverse_densite[posindexhab])))
                                cible->maladie = temps_infection;
                    }
                }
            }
    }


    nb_mort.add(getnbdeath());
    nb_sains.add(getnbsains());
    nb_gueris.add(getnbgueris());
    nb_infecte.add(getnbinfecte());
    hopital.add(act_hop);
    unsigned long date = nb_mort.date;
    if(date >= 10)
    {
        if(date % 9 == 0)
        {
            dmort.add(
                        (nb_mort.all.at(date-1))-
                        (nb_mort.all.at(date-10))
                        );
            dgueris.add(
                        (nb_gueris.all.at(date-1))-
                        (nb_gueris.all.at(date-10))
                        );
            dinfecte.add(
                        (nb_infecte.all.at(date-1))-
                        (nb_infecte.all.at(date-10))
                        );
        }
    }
    moyenne_age_mort.add(getmoyenneagemort());
    moyenne_age_infecte.add(getmoyenneageinfecte());
    if(++tranche_actuel == 9)
    {
        tranche_actuel = 0;
        jour++;
    }
}

char *Simulateur::getDate()
{
    int h = 0;
    switch(tranche_actuel)
    {
        case 0:
            h = 0;
            break;
        case 1:
            h = 7;
            break;
        case 2:
            h = 8;
            break;
        case 3:
            h = 12;
            break;
        case 4:
            h = 13;
            break;
        case 5:
            h = 14;
            break;
        case 6:
            h = 16;
            break;
        case 7:
            h = 18;
            break;
        case 8:
            h = 21;
            break;
    }

    sprintf(simblanket,"Jour %d : %dh00",jour,h);
    return simblanket;
}

long Simulateur::memory_taked()
{
    long result = sizeof(Simulateur);
    result +=
        #if Qt59
            im->byteCount();
#else
            im->sizeInBytes();
        #endif
    result += sizeof(humain)*nb;
    for(long i = 0;i != nb;++i)
        result += population[i].nbfamille*sizeof(Uint8);
    return result;
}

QVector<double> Simulateur::timeline()
{
     return all;
}
