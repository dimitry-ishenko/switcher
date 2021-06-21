////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"

#include <QApplication>
#include <QFile>
#include <QGSettings>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QVariant>

#include <csignal>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
Setting current()
{
    Setting s;

    {
        QGSettings gs { "org.gnome.system.proxy" };
        s.mode = gs.get("mode").toString();
        s.autoconfig_url = gs.get("autoconfig-url").toString();
        s.ignore_hosts = gs.get("ignore-hosts").toString();
    }

    for(auto const& type : Setting::types)
    {
        QGSettings gs { "org.gnome.system.proxy." + type.toLatin1() };
        Uri uri {
            gs.get("host").toString(),
            gs.get("port").toInt()
        };
        if(uri.is_valid()) s.uris[type] = std::move(uri);
    }

    return s;
}

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

    auto path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    QFile file { path + "/switcher.conf" };
    if(!file.open(QFile::ReadOnly))
    {
        auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
        throw std::runtime_error { err.toStdString() };
    }

    auto settings = Settings::from(file);

    QIcon none { ":/none.png" };
    QMenu menu;

    QSystemTrayIcon tray { none };
    tray.setContextMenu(&menu);
    tray.show();

    QTimer timer;
    timer.start(1000);

    QIcon::setThemeName("switcher");
    QObject::connect(&timer, &QTimer::timeout, [&]()
    {
        static QString prev_name;

        auto name = settings.match( current() );
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
