////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "profile.hpp"
#include "settings.hpp"

#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QStandardPaths>
#include <QSystemTrayIcon>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    QApplication app { argc, argv };

    std::signal(SIGINT, [](int) {
        std::cout << "Received SIGINT - stopping" << std::endl;
        QCoreApplication::quit();
    });
    std::signal(SIGTERM, [](int) {
        std::cout << "Received SIGTERM - stopping" << std::endl;
        QCoreApplication::quit();
    });

    ////////////////////
    auto path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    QFile file { path + "/switcher.conf" };
    if(!file.open(QFile::ReadOnly))
    {
        auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
        throw std::runtime_error { err.toStdString() };
    }

    auto entries = profile::read_from(file);

    ////////////////////
    QIcon::setThemeName("switcher");
    QIcon none { ":/none.png" };

    QSystemTrayIcon tray { none };
    QMenu menu;
    tray.setContextMenu(&menu);
    tray.show();

    ////////////////////
    settings s;

    auto update = [&]()
    {
        QIcon icon;

        auto current = s.current();
        for(auto const& [ name, entry ] : entries)
            if(entry == current)
            {
                icon = QIcon::fromTheme(name);
                break;
            }

        if(icon.isNull()) icon = none;
        tray.setIcon(icon);
    };
    update();

    QObject::connect(&s, &settings::changed, update);

    return app.exec();
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
