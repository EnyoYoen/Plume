#include "plume.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Plume *plume = new Plume();
    plume->show();

    int ret = a.exec();

    delete plume;

    return ret;
}