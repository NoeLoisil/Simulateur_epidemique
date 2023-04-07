#include <QApplication>
#include "main.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    programme *prog = new programme;
    prog->setup();
    return app.exec();
}
