////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"
#include <QVariant>

////////////////////////////////////////////////////////////////////////////////
settings::settings(QObject* parent) : QObject { parent }
{
    for(auto const& type : proxy::types)
    {
        auto gs = new QGSettings { proxy::schema_id + "." + type };
        types.emplace(type, gs);

        connect(gs, &QGSettings::changed, this, &settings::changed);
    }
    connect(&proxy, &QGSettings::changed, this, &settings::changed);
}

////////////////////////////////////////////////////////////////////////////////
settings::entry settings::current()
{
    entry e;
    e.mode = proxy.get(proxy::mode).toString();
    e.autoconfig_url = proxy.get(proxy::autoconfig_url).toString();
    e.ignore_hosts = proxy.get(proxy::ignore_hosts).toString();

    for(auto const& [ type, gs ] : types)
    {
        entry::uri uri {
            gs->get(proxy::host).toString(),
            gs->get(proxy::port).toInt(0)
        };
        if(uri.is_valid()) e.types[type] = std::move(uri);
    }

    return e;
}

////////////////////////////////////////////////////////////////////////////////
void settings::switch_to(const settings::entry& e)
{
    proxy.set(proxy::mode, e.mode);
    proxy.set(proxy::autoconfig_url, e.autoconfig_url);
    proxy.set(proxy::ignore_hosts, e.ignore_hosts);

    // clear all uris first
    for(auto const& [ _, type ] : types)
    {
        type->set(proxy::host, "");
        type->set(proxy::port, 0);
    }

    for(auto const& [ name, uri ] : e.types)
    {
        types[name]->set(proxy::host, uri.host);
        types[name]->set(proxy::port, uri.port);
    }
}
