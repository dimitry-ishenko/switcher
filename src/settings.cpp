////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
const std::set<Mode> Setting::modes { "none", "manual", "auto" };
const std::set<Type> Setting::types { "http", "https", "ftp", "socks" };

////////////////////////////////////////////////////////////////////////////////
Settings Settings::from_file(const QString& path)
{
    QFile file{ path };
    if(!file.open(QFile::ReadOnly))
    {
        auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
        throw std::runtime_error{ err.toStdString() };
    }

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
            else if(Setting::types.count(param))
            {
                auto values = entry[param].toObject();
                Uri uri {
                    values["host"].toString(),
                    values["port"].toInt(0)
                };
                if(uri.is_valid()) setting.uris[param] = std::move(uri);
            }
        }
    }

    return settings;
}

////////////////////////////////////////////////////////////////////////////////
bool Setting::operator==(const Setting& rhs) const
{
    if(mode != rhs.mode) return false;

         if(mode == "none") return true;
    else if(mode == "auto") return autoconfig_url == rhs.autoconfig_url;
    else if(mode == "manual")
    {
        if(ignore_hosts != rhs.ignore_hosts) return false;
        if(uris.size() != rhs.uris.size()) return false;

        for(auto const& [ type, uri ] : rhs.uris)
        {
            auto it = uris.find(type);
            if(it == uris.end() || !(it->second == uri)) return false;
        }
        return true;
    }
    else return false;
}

////////////////////////////////////////////////////////////////////////////////
QString Settings::match(const Setting& other)
{
    for(auto const& [ name, setting ] : *this) if(setting == other) return name;
    return QString{ };
}
