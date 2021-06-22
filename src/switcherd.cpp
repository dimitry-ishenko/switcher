////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "proxy.hpp"
#include "settings.hpp"

#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QGSettings>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QVariant>

#include <csignal>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
Setting current()
{
    Setting s;

    {
        QGSettings gs { proxy::schema_id };
        s.mode = gs.get("mode").toString();
        s.autoconfig_url = gs.get("autoconfig-url").toString();
        s.ignore_hosts = gs.get("ignore-hosts").toString();
    }

    for(auto const& type : proxy::types)
    {
        QGSettings gs { QByteArray { proxy::schema_id } + "." + type };
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

    ////////////////////
    auto path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    QFile file { path + "/switcher.conf" };
    if(!file.open(QFile::ReadOnly))
    {
        auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
        throw std::runtime_error { err.toStdString() };
    }

    auto settings = Settings::from(file);

    ////////////////////
    QIcon::setThemeName("switcher");
    QIcon none { ":/none.png" };

    QSystemTrayIcon tray { none };
    QMenu menu;
    tray.setContextMenu(&menu);
    tray.show();

    ////////////////////
    QGSettings proxy { proxy::schema_id };
    std::vector<QGSettings*> types;

    auto update = [&]()
    {
        auto name = settings.match( current() );

        auto icon = QIcon::fromTheme(name);
        if(icon.isNull()) icon = none;

        tray.setIcon(icon);
    };

    QObject::connect(&proxy, &QGSettings::changed, update);
    for(auto const& type : proxy::types)
    {
        types.push_back(new QGSettings { QByteArray { proxy::schema_id } + "." + type, { }, &proxy });
        QObject::connect(types.back(), &QGSettings::changed, update);
    }

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
