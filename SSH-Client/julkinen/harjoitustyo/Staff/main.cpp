#include <QApplication>
#include "Initialization.h"

#ifdef USE_SSH
#include <iostream>
#include <libssh2.h>
#include "loginwindow.h"

using namespace Staff;
int main(int argc, char *argv[])
{
    if (libssh2_init(0))
    {
        std::cerr << "Failed to init libssh2\n";
        return 1;
    }

    QApplication a(argc, argv);
    LoginWindow w;
    w.exec();
    if (w.hasConnection())
    {
        auto connections = w.connections();
        QMainWindow * window = FSInterface::startMain(connections);
        window->show();
        int res = a.exec();
        delete window;
        return res;
    } else
        return 0;
}

#else
#include "virtualconnection.h"
using namespace Staff;
using namespace FSInterface;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QList<std::shared_ptr<ConnectionInterface>> connections;
    connections.append(empty());
    connections.append(hello());
    QMainWindow * window = FSInterface::startMain(connections);
    window->show();
    int res = a.exec();
    delete window;
    return res;
}

#endif
