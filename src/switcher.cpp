////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "switcher.hpp"
#include <QByteArray>
#include <QGSettings>
#include <QVariant>

////////////////////////////////////////////////////////////////////////////////
void Switcher::switch_to(const Setting& setting)
{
    {
        QGSettings gs{ "org.gnome.system.proxy" };
        gs.set("mode", setting.mode);
        gs.set("autoconfig-url", setting.autoconfig_url);
        gs.set("ignore-hosts", setting.ignore_hosts);
    }

    for(auto const& [type, uri] : setting.uris)
    {
        QGSettings gs{ ("org.gnome.system.proxy." + type).toLatin1() };
        gs.set("host", uri.host);
        gs.set("port", uri.port);
    }
}

////////////////////////////////////////////////////////////////////////////////
Setting Switcher::get_current()
{
    Setting setting;

    {
        QGSettings gs{ "org.gnome.system.proxy" };
        setting.mode = gs.get("mode").toString();
        setting.autoconfig_url = gs.get("autoconfig-url").toString();
        setting.ignore_hosts = gs.get("ignore-hosts").toString();
    }

    for(auto const& type : Setting::types)
    {
        QGSettings gs{ QByteArray{ } + "org.gnome.system.proxy." + type };
        Uri uri {
            gs.get("host").toString(),
            gs.get("port").toInt()
        };
        if(uri.host.size() && uri.port) setting.uris[type] = std::move(uri);
    }

    return setting;
}
