////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "proxy.hpp"
#include "settings.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
bool profile::operator==(const profile& rhs) const
{
    if(mode != rhs.mode) return false;

         if(mode == "none") return true;
    else if(mode == "auto") return autoconfig_url == rhs.autoconfig_url;
    else if(mode == "manual")
    {
        if(ignore_hosts != rhs.ignore_hosts) return false;
        if(types.size() != rhs.types.size()) return false;

        for(auto const& [ type, uri ] : rhs.types)
        {
            auto it = types.find(type);
            if(it == types.end() || !(it->second == uri)) return false;
        }
        return true;
    }
    else return false;
}

////////////////////////////////////////////////////////////////////////////////
Settings read_from(QFile& file)
{
    Settings settings;

    auto entries = QJsonDocument::fromJson(file.readAll()).object();
    for(auto const& name : entries.keys())
    {
        auto& setting = settings[name];

        auto entry = entries[name].toObject();
        for(auto const& param : entry.keys())
        {
                 if(param == "mode") setting.mode = entry[param].toString();
            else if(param == "autoconfig_url") setting.autoconfig_url = entry[param].toString();
            else if(param == "ignore_hosts") setting.ignore_hosts = entry[param].toString();
            else if(std::count(proxy::types.begin(), proxy::types.end(), param))
            {
                auto values = entry[param].toObject();
                uri uri {
                    values["host"].toString(),
                    values["port"].toInt(0)
                };
                if(uri.is_valid()) setting.types[param] = std::move(uri);
            }
        }
    }

    return settings;
}
