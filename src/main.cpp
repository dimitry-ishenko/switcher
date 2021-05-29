////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"

#include <QByteArray>
#include <QGSettings>
#include <QStandardPaths>
#include <QVariant>

#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
void switch_to(const Setting& setting)
{
    {
        QGSettings gs{ "org.gnome.system.proxy" };
        gs.set("mode", setting.mode);
        gs.set("autoconfig-url", setting.autoconfig_url);
        gs.set("ignore-hosts", setting.ignore_hosts);
    }

    // clear all uris first
    for(auto const& type : Setting::types)
    {
        QGSettings gs{ QByteArray{ } + "org.gnome.system.proxy." + type };
        gs.set("host", "");
        gs.set("port", 0);
    }

    for(auto const& [type, uri] : setting.uris)
    {
        QGSettings gs{ ("org.gnome.system.proxy." + type).toLatin1() };
        gs.set("host", uri.host);
        gs.set("port", uri.port);
    }
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    if(argc == 2)
    {
        auto path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
        auto settings = Settings::from_file(path + "/switcher.conf");

        auto it = settings.find(argv[1]);
        if(it == settings.end()) return 1;

        switch_to(it->second);
        return 0;
    }

    std::cout << "Usage: switcher <conf>\n" << std::endl;
    throw std::invalid_argument{ "Invalid number of parameters" };
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
