#include <QCoreApplication>
#include "dcunit.h"
//#include "networkmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DCunit var;
    //NetworkManager serv (9090, "localhost");

    return a.exec();
}
