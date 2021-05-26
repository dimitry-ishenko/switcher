////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "switcher.hpp"

#include <QFile>
#include <QGSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
Switcher::Switcher(const QString& name, QObject* parent) :
    QObject{ parent }
{
    auto path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];
    QFile file{ path + "/" + name + ".conf" };

    if(!file.open(QFile::ReadOnly))
    {
        auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
        throw std::runtime_error{ err.toStdString() };
    }

    auto ents = QJsonDocument::fromJson(file.readAll()).object();
    for(auto const& conf : ents.keys())
    {
        auto& set = settings[conf];

        auto ent = ents[conf].toObject();
        for(auto const& name : ent.keys())
        {
            if(name == "mode")
            {
                set.mode = ent[name].toString();
            }
            else if(name == "autoconfig_url")
            {
                set.autoconfig_url = ent[name].toString();
            }
            else if(name == "ignore_hosts")
            {
                set.ignore_hosts = ent[name].toString();
            }
            else if(name == "http" || name == "https" || name == "ftp" || name == "socks")
            {
                auto val = ent[name].toObject();
                set.uris[name].host = val["host"].toString();
                set.uris[name].port = val["port"].toInt(0);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void Switcher::switch_to(const QString& name)
{
    auto it = settings.find(name);
    if(it != settings.end())
    {
        auto const& set = it->second;

        {
            QGSettings gs{ "org.gnome.system.proxy" };
            gs.set("mode", set.mode);
            gs.set("autoconfig-url", set.autoconfig_url);
            gs.set("ignore-hosts", set.ignore_hosts);
        }

        for(auto const& [type, uri] : set.uris)
        {
            QGSettings gs{ ("org.gnome.system.proxy." + type).toLatin1() };
            gs.set("host", uri.host);
            gs.set("port", uri.port);
        }
    }
}
