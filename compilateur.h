#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include "constant.h"

class compilateur : public QWidget
{
    Q_OBJECT

    public:

    bool Winlayout();

    void add(const char* text);
    void add(const QString text);
    void add(const QString text,int value);
    void add(const QString text,Uint64 value);
    void add(const QString text,double value);
    void add(const QString text,int value1,int value2);
    void add(const QString text,int value1,int value2,int value3);
    void add(const QString text,int value,int *tab,long nb);
    void add(const QString text,int value1,int value2,int *tab,long nb);
    void add(const QString text,Uint64 *tab,long nb);

    public slots:


    public:

    QWidget *fenetre;
    QVBoxLayout *Vertical;
    QHBoxLayout* Hori1;
    QHBoxLayout* Hori2;
    QTextEdit *output;

};

#endif // COMPILATEUR_H
