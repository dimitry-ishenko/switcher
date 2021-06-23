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
namespace profile
{

////////////////////////////////////////////////////////////////////////////////
bool entry::operator==(const entry& rhs) const
{
    if(mode != rhs.mode) return false;

         if(mode == none) return true;
    else if(mode == auto_) return autoconfig_url == rhs.autoconfig_url;
    else if(mode == manual)
    {
        if(ignore_hosts != rhs.ignore_hosts) return false;
        if(types.size() != rhs.types.size()) return false;

        for(auto const& [ type, ruri ] : rhs.types)
        {
            auto it = types.find(type);
            if(it == types.end()) return false;

            auto const& luri = it->second;
            if(luri.host != ruri.host || luri.port != ruri.port) return false;
        }
        return true;
    }
    else return false;
}

////////////////////////////////////////////////////////////////////////////////
entries read_from(QFile& file)
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

                if(uri.is_valid()) e.types[name] = std::move(uri);
            }
        }
    }

    return entries;
}

////////////////////////////////////////////////////////////////////////////////
}
