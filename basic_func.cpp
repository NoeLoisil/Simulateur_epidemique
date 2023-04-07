#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QThread>
#include "constant.h"

char cc,cc2;
char *cblanket = new char[1023];
long trom;
Sint64 tpm;
double ftpm;


#include "data_noms.h"
extern const QString nom_data[];
extern const QString prenom_data[];




QString nom_random(long pos)
{
    return prenom_data[pos%DATA_SIZE_PRENOM]+nom_data[pos%DATA_SIZE_NOM];
}


bool charcpm(char *target,char *chaine)
{
    return (*(unsigned char *)--target - *(unsigned char *)--chaine);
}

Uint32 rand32()
{
    return rand()*RAND_MAX+rand();
}

Uint64 rand64()
{
    return ((Uint64)rand32())*RAND_MAX*RAND_MAX+rand32();
}

bool alea(double x)
{
    return rand() < RAND_MAX*x;
}

bool alea32(double x)
{
    return rand32() < RAND_MAX32*x;
}

bool alea64(double x)
{
    return rand64() < x*RAND_MAX64;
}


QVector<Uint32> Blur(QVector<Uint32> input,unsigned int nb,unsigned int width)
{
    unsigned int size = input.size();
    unsigned int height = size/width;
    unsigned int widht_min = width-1;
    QVector<Uint32> output(input.size());
    for(unsigned int i = 0;i != nb;++i)
    {
        for(unsigned int index = 0;index != size;++index)
            output[index] = input.at(index)*4;
        for(unsigned int index = width;index != size;++index)
        {
            output[index] += input.at(index-width);
            output[index-width] += input.at(index);
        }
        unsigned int index = 1;
        unsigned int index2 = 0;
        for(unsigned int h = 0;h != height;++h)
        {
            for(unsigned int j = 0;j != widht_min;++j)
            {
                output[index] += input.at(index-1);
                output[index2] += input.at(index2+1);
                index++;
                index2++;
            }
            index++;
            index2++;
        }
        for(unsigned int index = 0;index != size;++index)
            input[index] = output.at(index)/8;
    }
    return input;
}

QVector<Uint32> Blur8(QVector<Uint32> input,unsigned int nb,unsigned int width)
{
    unsigned int size = input.size();
    unsigned int height = size/width;
    unsigned int widht_min = width-1;
    QVector<Uint32> output(input.size());
    for(unsigned int i = 0;i != nb;++i)
    {
        for(unsigned int index = 0;index != size;++index)
            output[index] = input.at(index)*4;
        for(unsigned int index = width;index != size;++index)
        {
            output[index] += input.at(index-width);
            output[index-width] += input.at(index);
        }
        unsigned int index = 1;
        unsigned int index2 = 0;
        for(unsigned int h = 0;h != height;++h)
        {
            for(unsigned int j = 0;j != widht_min;++j)
            {
                output[index] += input.at(index-1);
                output[index2] += input.at(index2+1);
                index++;
                index2++;
            }
            index++;
            index2++;
        }
        for(unsigned int index = 0;index != size;++index)
            input[index] = output.at(index);
    }
    return input;
}
QVector<Uint64> Blur(QVector<Uint64> input,unsigned int nb,unsigned int width)
{
    unsigned int size = input.size();
    unsigned int height = size/width;
    unsigned int widht_min = width-1;
    QVector<Uint64> output(input.size());
    for(unsigned int i = 0;i != nb;++i)
    {
        for(unsigned int index = 0;index != size;++index)
            output[index] = input.at(index)*4;
        for(unsigned int index = width;index != size;++index)
        {
            output[index] += input.at(index-width);
            output[index-width] += input.at(index);
        }
        unsigned int index = 1;
        unsigned int index2 = 0;
        for(unsigned int h = 0;h != height;++h)
        {
            for(unsigned int j = 0;j != widht_min;++j)
            {
                output[index] += input.at(index-1);
                output[index2] += input.at(index2+1);
                index++;
                index2++;
            }
            index++;
            index2++;
        }
        for(unsigned int index = 0;index != size;++index)
            input[index] = output.at(index)/8;
    }
    return input;
}

QVector<Uint64> Blur8(QVector<Uint64> input,unsigned int nb,unsigned int width)
{
    unsigned int size = input.size();
    unsigned int height = size/width;
    unsigned int widht_min = width-1;
    QVector<Uint64> output(input.size());
    for(unsigned int i = 0;i != nb;++i)
    {
        for(unsigned int index = 0;index != size;++index)
            output[index] = input.at(index)*4;
        for(unsigned int index = width;index != size;++index)
        {
            output[index] += input.at(index-width);
            output[index-width] += input.at(index);
        }
        unsigned int index = 1;
        unsigned int index2 = 0;
        for(unsigned int h = 0;h != height;++h)
        {
            for(unsigned int j = 0;j != widht_min;++j)
            {
                output[index] += input.at(index-1);
                output[index2] += input.at(index2+1);
                index++;
                index2++;
            }
            index++;
            index2++;
        }
        for(unsigned int index = 0;index != size;++index)
            input[index] = output.at(index);
    }
    return input;
}

void pass_char(FILE *file,char *ctp)
{
    do *ctp = fgetc(file); while(*ctp != '\n' && *ctp != -1);
}

bool get_line(FILE *file,char *found,char *stop)
{
    do
    {
        trom = ftell(file);
        cc = fgetc(file);
        if(cc == found[0])
        {
            fseek(file,trom,SEEK_SET);
            fscanf(file,"%s",cblanket);
            if(!strcmp(cblanket,found))
            {
                return 1;
            }
        }
        if(stop != NULL)
        {
            if(cc == stop[0])
            {
                fseek(file,trom,SEEK_SET);
                fscanf(file,"%s",cblanket);
                if(!strcmp(cblanket,stop))
                {
                    fseek(file,trom,SEEK_SET);
                    return 0;
                }
            }
        }
        cc2 = 0;
        if(cc != '\n' && cc != -1)
            pass_char(file,&cc2);
    }while(cc2 != -1 && cc != -1);
    return 0;
}

Sint64 take_long(FILE* file)
{
    cc = fgetc(file);
    while(cc != '=')
        cc = fgetc(file);
    fscanf(file," %lld",&tpm);
    return tpm;
}

float take_float(FILE* file)
{
    cc = fgetc(file);
    while(cc != '=')
        cc = fgetc(file);
    fscanf(file," %lf",&ftpm);
    return ftpm;
}



void take_chaine(FILE *file,char *target,int max)
{
    trom = 0; cc = fgetc(file);
    cc = fgetc(file);
    while(cc != '"')
        cc = fgetc(file);
    cc = fgetc(file);
    while(cc != '"' && trom != max)
    {
        target[trom] = cc;
        cc = fgetc(file);
        ++trom;
    }
    target[trom] = '\0';
}


FILE* tfopen(const char *filename)
{
    /*FILE* retour = NULL;
    retour = fopen(filename,"r");
    trom = 0;
    while(trom < 4 && retour == NULL)
    {
        retour = fopen(filename,"r");
        trom++;
        QThread::sleep(1);
    }
    return retour;*/
    return fopen(filename,"r");
}

void bool_taker(FILE* file,const char *variable,bool *target)
{
    fscanf(file,variable);
    fscanf(file,"%s",cblanket);

    if(strcmp(cblanket,"true"))
        *target = 0;
    else
        *target = 1;
    getc(file);
}

int borne(int value,int min,int max)
{
    if(value > max) return max;
    if(value < min) return min;
    return value;
}
