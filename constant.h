#ifndef CONSTANT_H
#define CONSTANT_H

#include <stdio.h>
#include <stdlib.h>
#include <QtWidgets>
#include <bits/stdc++.h>
#include "C:/qcustomplot/qcustomplot.h"
using namespace std;

#ifndef datasize
#define datasize
#define par_tranche 1
#define par_jour 9
#define par_semaine 63
#define par_mois 270
#define par_ans 3285

#define data_delta_all par_tranche
#define data_delta_all_jour par_jour
#define data_size_all par_ans*10/data_delta_all
#define data_size_all_jour par_ans*100/data_delta_all_jour
#endif

#define RAND_MAX32 0x3FFF0001
#define RAND_MAX64 RAND_MAX32*RAND_MAX32

#if QT_VERSION >= 0x051000
#define Qt59 false
#else
#define Qt59 false
#endif

#define max(a,b) a > b ? a : b
#define min(a,b) a > b ? b : a
typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short int Uint16;
typedef signed short int Sint16;
typedef unsigned long Uint32;
typedef signed long Sint32;
typedef unsigned long long Uint64;
typedef signed long long Sint64;

bool charcpm(char *target,char *chaine);
void pass_char(FILE *file,char *ctp);
void take_chaine(FILE *file,char *target,int max);
void bool_taker(FILE* file,const char *variable,bool *target);
QString nom_random(long pos);
Sint64 take_long(FILE* file);
float take_float(FILE* file);
FILE* tfopen(const char *filename);
int borne(int value,int min,int max);
QVector<Uint32> Blur(QVector<Uint32> input,unsigned int nb,unsigned int width);
QVector<Uint32> Blur8(QVector<Uint32> input,unsigned int nb,unsigned int width);
QVector<Uint64> Blur(QVector<Uint64> input,unsigned int nb,unsigned int width);
QVector<Uint64> Blur8(QVector<Uint64> input,unsigned int nb,unsigned int width);
bool alea(double x);
bool alea32(double x);
bool alea64(double x);


Uint32 rand32();
Uint64 rand64();
#define challoc(x) (char*)malloc(x)


typedef Uint8 color;
typedef long pourcentage;


typedef Uint8 MapType;
enum{MAP_POPULATION,MAP_CONTAMINATION_ABS,MAP_CONTAMINATION_REL,MAP_MORT,MAP_PROXI,MAP_DERIVE_COONTAMINATION,MAP_DERIVE_MORT};
typedef Uint8 GraphType;
enum{GRAPH_CONTAMINATION_TOTAL,GRAPH_CONTAMINATION_JOURNALIER,GRAPH_AGE_CONTAMINATION,GRAPH_HOPITAL};
typedef Uint8 ParaType;
enum{PARA_NONE,PARA_MESURE,PARA_VIRUS};


class preferences
{
public:
    bool gpu;
    bool automatique;
    bool ram;
    bool multi_thread;
    const char* world_file;
    const char* ssd_file;
    QString save_file;
    preferences(){gpu = false;automatique = true;ram = true;multi_thread = false;world_file = "C:\\world";ssd_file = "C:/population";save_file.fromStdString(world_file);}
    void load()
    {
        FILE* file = fopen("options.txt","r");
        if(file && false)
        {
            delete world_file;world_file = challoc(256);
            fscanf(file,"world_file = %s\n",(char *)world_file);
            bool_taker(file,"automatique = ",&automatique);
            bool_taker(file,"ram = ",&ram);
            bool_taker(file,"multi_thread = ",&multi_thread);
            bool_taker(file,"gpu = ",&gpu);
            save_file.fromStdString(world_file);
            fclose(file);
        }
        else
            preferences();
    }

    void save()
    {
        FILE* file = fopen("options.txt","w+");
        fprintf(file,"world_file = %s\nautomatique = ",world_file);
        write_bool(file,automatique);
        fprintf(file,"\nram = ");
        write_bool(file,ram);
        fprintf(file,"\nmulti_thread = ");
        write_bool(file,multi_thread);
        fprintf(file,"\ngpu = ");
        write_bool(file,gpu);
        fclose(file);
    }
    Uint8 pref_memory;

private:

    void write_bool(FILE *file,bool x)
    {
        if(x) fprintf(file,"true");
        else fprintf(file,"false");
    }
};


#endif // CONSTANT_H

#undef NULL
#define NULL nullptr
