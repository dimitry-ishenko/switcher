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
        QGSettings gs { proxy::schema_id };
        gs.set(proxy::mode, setting.mode);
        gs.set(proxy::autoconfig_url, setting.autoconfig_url);
        gs.set(proxy::ignore_hosts, setting.ignore_hosts);
    }

    // clear all uris first
    for(auto const& type : proxy::types)
    {
        QGSettings gs { proxy::schema_id + "." + type };
        gs.set(proxy::host, "");
        gs.set(proxy::port, 0);
    }

    for(auto const& [type, uri] : setting.uris)
    {
        QGSettings gs { proxy::schema_id + "." + type.toLatin1() };
        gs.set(proxy::host, uri.host);
        gs.set(proxy::port, uri.port);
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

        auto settings = read_from(file);

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
