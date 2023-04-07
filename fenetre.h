#ifndef FENETRE_H
#define FENETRE_H

#include "constant.h"

class Fenetre : public QWidget
{
    Q_OBJECT
public:
    Fenetre();

    void setImportant(bool value)
    {
        important = value;
    }

    bool important;

protected:
    void closeEvent(QCloseEvent *event)
    {
        if(important)
        {
            QMessageBox* Box = new QMessageBox();
            QMessageBox::StandardButton r = Box->question(this,"Attention","Voulez-vous vraiment fermer cette fenêtre ?");
            delete Box;
            if(r)
            {
                emit closing();
                event->accept();
            }
            else
                event->ignore();
        }
        else
        {
            emit closing();
            event->accept();
        }
    }
public:
signals:
    void closing();
};

#endif // FENETRE_H
