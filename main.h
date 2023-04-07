#ifndef MAIN_H
#define MAIN_H

#include "primary.h"
#include "world.h"
extern preferences SETTINGS;

class programme : public QWidget
{
    Q_OBJECT

    public:

    void setup()
    {
        SETTINGS = new preferences();
        SETTINGS->load();
        choose = new primary;
        choose->SETTINGS = SETTINGS;
        choose->WinLayout();
        connect(choose->button,SIGNAL(clicked()),this,SLOT(generate_world()));
    }

    public slots:

    void free_layout();

    void generate_world()
    {
        SETTINGS->gpu = choose->gpu->isChecked();
        SETTINGS->ram = choose->ram->isChecked() || choose->ram_multithread->isChecked();
        SETTINGS->multi_thread = choose->ram_multithread->isChecked();
        SETTINGS->automatique = choose->automatique->isChecked();
        SETTINGS->save();
        if(!compiler())
        {

        }
    }

    bool exit()
    {
        if(Fworld != NULL)
            delete Fworld;
        Fworld = NULL;
        compil->add("La mémoire a bien été libéré lors du crash\n");
        QMessageBox* Box = new QMessageBox();
        Box->critical(compil->fenetre,"plantage du compilateur","Crash lors de la compilation, verifiez vos fichiers");
        delete Box;
        return true;
    }


    bool compiler()
    {
        compil = new compilateur;
        //QString dossier = QFileDialog::getExistingDirectory(nullptr,QString(),"C:\\Users\\noelo\\Desktop\\data");
        //clock_t stop,start = clock();
        compil->Winlayout();
        Fworld = new world(SETTINGS);
        //compil->add("Début de la création...\n");
        Fworld->setup(compil);
        //stop = clock();
        //int s = (stop-start)/CLOCKS_PER_SEC;
        //int ms = (((stop-start)*1000)/CLOCKS_PER_SEC)-(s*1000);

        //sprintf(blanket,"Création terminé, temps de création : %d.%0*d seconde(s)\n",s,3,ms);
        //compil->add(blanket);
        choose->fenetre->hide();
        delete choose;
        return 0;
    }

    private:

    primary *choose;
    compilateur *compil;

    world *Fworld = NULL;
    preferences *SETTINGS;

};


#endif
