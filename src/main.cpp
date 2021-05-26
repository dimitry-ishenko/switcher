////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "switcher.hpp"

#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>

#include <csignal>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
void quit(int signal)
{
    std::cout << "Received signal " << signal << " - " << "stopping" << std::endl;
    QCoreApplication::quit();
}

////////////////////////////////////////////////////////////////////////////////
int run_daemon(int argc, char* argv[])
{
    QCoreApplication app{ argc, argv };

    std::signal(SIGINT, [](int) {
        std::cout << "Received SIGINT - stopping" << std::endl;
        QCoreApplication::quit();
    });
    std::signal(SIGTERM, [](int) {
        std::cout << "Received SIGTERM - stopping" << std::endl;
        QCoreApplication::quit();
    });

    Switcher switcher{ NAME };

    QLocalServer server;
    if(!server.listen(NAME))
    {
        auto err = "Can't create socket - " + server.errorString();
        throw std::runtime_error{ err.toStdString() };
    }

    QObject::connect(&server, &QLocalServer::newConnection, [&]()
    {
        if(auto sock = server.nextPendingConnection(); sock)
        {
            sock->waitForReadyRead();
            switcher.switch_to(sock->readAll());
            delete sock;
        }
    });

    return app.exec();
}

////////////////////////////////////////////////////////////////////////////////
int switch_to(const QByteArray& conf)
{
    QLocalSocket sock;

    sock.connectToServer(NAME);
    if(!sock.waitForConnected())
    {
        auto err = "Can't connect to server - " + sock.errorString();
        throw std::runtime_error{ err.toStdString() };
    }

    sock.write(conf);
    if(!sock.waitForBytesWritten())
    {
        auto err = "Can't communicate to server - " + sock.errorString();
        throw std::runtime_error{ err.toStdString() };
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    switch(argc)
    {
    case 2: return switch_to(argv[1]); break;
    case 1: return run_daemon(argc, argv); break;
    }

    std::cout << "Usage: " << NAME << " [conf]\n" << std::endl;
    throw std::invalid_argument{ "Invalid number of parameters" };
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
catch(...)
{
    std::cerr << "???" << std::endl;
    return 1;
}
