#include "compilateur.h"
#include "main.h"

char *buffer_compil = challoc(2048);
int var_compil;
#define COMPIL_DEBUG false
bool compilateur::Winlayout()
{
    fenetre = new QWidget();
    fenetre->setMinimumSize(500,200);
    fenetre->resize(500,300);
    Vertical = new QVBoxLayout(fenetre);
    Hori2 = new QHBoxLayout();
    output = new QTextEdit();
    Vertical->addWidget(output);
    Vertical->addLayout(Hori2);
    output->setReadOnly(1);
    output->setText("");
    output->setToolTip("Logs");
    fenetre->show();
    return 0;
}

void compilateur::add(const char* text)
{
#if COMPIL_DEBUG
    qDebug("%s",text);
#endif
    output->insertPlainText(text);
}

void compilateur::add(const QString text)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
}

void compilateur::add(const QString text,int value)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%d\n",value);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
}

void compilateur::add(const QString text,Uint64 value)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%llu\n",value);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);

}

void compilateur::add(const QString text,double value)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%lf\n",value);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
}

void compilateur::add(const QString text,int value1,int value2)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%d %d\n",value1,value2);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
}

void compilateur::add(const QString text,int value1,int value2,int value3)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%d %d %d\n",value1,value2,value3);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
}

void compilateur::add(const QString text,int value,int *tab,long nb)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%d",value);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
    for(var_compil = 0;var_compil != nb;var_compil++)
    {
        sprintf(buffer_compil," %d",tab[var_compil]);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
        output->insertPlainText((QString)buffer_compil);
    }
    output->insertPlainText("\n");

}
void compilateur::add(const QString text,int value1,int value2,int *tab,long nb)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    output->insertPlainText(text);
    sprintf(buffer_compil,"%d %d",value1,value2);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
    output->insertPlainText((QString)buffer_compil);
    for(var_compil = 0;var_compil != nb;var_compil++)
    {
        sprintf(buffer_compil," %d",tab[var_compil]);
        output->insertPlainText((QString)buffer_compil);
    }
    output->insertPlainText("\n");
}

void compilateur::add(const QString text,Uint64 *tab,long nb)
{
#if COMPIL_DEBUG
    qDebug("%s", text.toStdString().c_str());
#endif
    for(var_compil = 0;var_compil != nb;var_compil++)
    {
        sprintf(buffer_compil," %llu",tab[var_compil]);
#if COMPIL_DEBUG
    qDebug("%s", buffer_compil);
#endif
        output->insertPlainText((QString)buffer_compil);
    }
    output->insertPlainText("\n");
}

void programme::free_layout()
{
    delete compil->Hori1;
    delete compil->Hori2;
    delete compil->output;
    delete compil->fenetre;
    delete compil->Vertical;
    delete compil;
}

