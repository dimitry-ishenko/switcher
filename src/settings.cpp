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
    using namespace proxy;

    entry e;
    e.mode = proxy.get(mode).toString();
    e.autoconfig_url = proxy.get(autoconfig_url).toString();
    e.ignore_hosts = proxy.get(ignore_hosts).toString();

    for(auto const& [ type, gs ] : types)
    {
        entry::uri uri { gs->get(host).toString(), gs->get(port).toInt(0) };
        if(uri.is_valid()) e.types[type] = std::move(uri);
    }

    return e;
}
