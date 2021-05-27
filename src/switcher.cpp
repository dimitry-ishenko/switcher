////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "switcher.hpp"
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
