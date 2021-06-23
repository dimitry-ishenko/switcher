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
            auto ti = types.find(type);
            if(ti == types.end()) return false;

            auto const& luri = ti->second;
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

    auto file_json = QJsonDocument::fromJson(file.readAll()).object();
    for(auto fi = file_json.begin(); fi != file_json.end(); ++fi)
    {
        auto& entry = entries[fi.key()];

        auto entry_json = fi.value().toObject();
        for(auto ei = entry_json.begin(); ei != entry_json.end(); ++ei)
        {
            auto key = ei.key();
            auto value = ei.value();

                 if(key == mode) entry.mode = value.toString();
            else if(key == autoconfig_url) entry.autoconfig_url = value.toString();
            else if(key == ignore_hosts) entry.ignore_hosts = value.toString();
            else if(std::count(types.begin(), types.end(), key))
            {
                auto uri_json = value.toObject();
                entry::uri uri { uri_json[host].toString(), uri_json[port].toInt(0) };

                if(uri.is_valid()) entry.types[key] = std::move(uri);
            }
        }
    }

    return entries;
}

////////////////////////////////////////////////////////////////////////////////
}
