#ifndef LOADING_BAR_H
#define LOADING_BAR_H

#include "compilateur.h"


class loading_bar : public QWidget
{
    Q_OBJECT
public:
    loading_bar(QWidget *parent = nullptr);


    QGridLayout *box;
    QProgressBar *bar;
    QLabel *frac;
    QLabel *etat;
    //QLabel *time;

    Uint64 final;
    //clock_t last_actualise;
    //double moy;

protected:
    /*void closeEvent(QCloseEvent *event)
        {event->ignore();}*/

public slots:
    void reset(Uint64 size,QString state = NULL);
    void actualise(Uint64 complet,QString state = NULL);
    void end();
};

#endif // LOADING_BAR_H
