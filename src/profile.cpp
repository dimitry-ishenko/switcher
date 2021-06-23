////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "profile.hpp"

#include <QJsonDocument>
#include <QJsonObject>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
profile::entries profile::read_from(QFile& file)
{
    entries entries;

    auto doc = QJsonDocument::fromJson(file.readAll()).object();
    for(auto const& name : doc.keys())
    {
        auto& e = entries[name];

        auto val = doc[name].toObject();
        for(auto const& name : val.keys())
        {
                 if(name == mode) e.mode = val[name].toString();
            else if(name == autoconfig_url) e.autoconfig_url = val[name].toString();
            else if(name == ignore_hosts) e.ignore_hosts = val[name].toString();
            else if(std::count(types.begin(), types.end(), name))
            {
                auto type = val[name].toObject();
                entry::uri uri { type[host].toString(), type[port].toInt(0) };

                if(uri.host.size() && uri.port > 0) e.types[name] = std::move(uri);
            }
        }
    }

    return entries;
}
