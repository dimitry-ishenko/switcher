////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"
#include "switcher.hpp"

#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QTimer>

#include <csignal>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
int run_daemon(int argc, char* argv[])
{
    QApplication app{ argc, argv };

    std::signal(SIGINT, [](int) {
        std::cout << "Received SIGINT - stopping" << std::endl;
        QCoreApplication::quit();
    });
    std::signal(SIGTERM, [](int) {
        std::cout << "Received SIGTERM - stopping" << std::endl;
        QCoreApplication::quit();
    });

    auto config_path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    auto settings = Settings::from_file(config_path + "/" + NAME + ".conf");

    QIcon none{ ":/none.png" };
    QMenu menu;

    QSystemTrayIcon tray{ none };
    tray.setContextMenu(&menu);
    tray.show();

    QTimer timer;
    timer.start(1000);

    QIcon::setThemeName(NAME);
    QObject::connect(&timer, &QTimer::timeout, [&]()
    {
        static QString prev_name;

        auto setting = Switcher::get_current();
        auto name = settings.match(setting);
        if(name != prev_name)
        {
            auto icon = QIcon::fromTheme(name);
            if(icon.isNull()) icon = none;

            tray.setIcon(icon);
            prev_name = name;
        }
    });

    return app.exec();
}

////////////////////////////////////////////////////////////////////////////////
int switch_to(const QByteArray& conf)
{
    auto config_path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    auto settings = Settings::from_file(config_path + "/" + NAME + ".conf");

    auto it = settings.find(conf);
    if(it == settings.end()) return 1;

    Switcher::switch_to(it->second);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
         if(argc == 2) return switch_to(argv[1]);
    else if(argc == 1) return run_daemon(argc, argv);
    else
    {
        std::cout << "Usage: " << NAME << " [conf]\n" << std::endl;
        throw std::invalid_argument{ "Invalid number of parameters" };
    }
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
