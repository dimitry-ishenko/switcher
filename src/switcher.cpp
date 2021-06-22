////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "proxy.hpp"
#include "settings.hpp"

#include <QFile>
#include <QGSettings>
#include <QStandardPaths>
#include <QVariant>

#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
void switch_to(const Setting& setting)
{
    {
        QGSettings gs { proxy::url };
        gs.set("mode", setting.mode);
        gs.set("autoconfig-url", setting.autoconfig_url);
        gs.set("ignore-hosts", setting.ignore_hosts);
    }

    // clear all uris first
    for(auto const& type : proxy::types)
    {
        QGSettings gs { QByteArray { proxy::url } + "." + type };
        gs.set("host", "");
        gs.set("port", 0);
    }

    for(auto const& [type, uri] : setting.uris)
    {
        QGSettings gs { QByteArray { proxy::url } + "." + type.toLatin1() };
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
        auto const path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];

        QFile file { path + "/switcher.conf" };
        if(!file.open(QFile::ReadOnly))
        {
            auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
            throw std::runtime_error { err.toStdString() };
        }

        auto settings = Settings::from(file);

        auto it = settings.find(argv[1]);
        if(it == settings.end()) return 1;

        switch_to(it->second);
        return 0;
    }

    std::cout << "Usage: switcher <conf-name>\n" << std::endl;
    throw std::invalid_argument { "Invalid number of parameters" };
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
