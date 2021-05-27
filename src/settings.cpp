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

#include <algorithm>
#include <iterator>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
bool contains(const T& cont, const U& value)
{
    using std::begin; using std::end;
    return std::find(begin(cont), end(cont), value) != end(cont);
}

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
                 if(param == "mode"          ) setting.mode = entry[param].toString();
            else if(param == "autoconfig_url") setting.autoconfig_url = entry[param].toString();
            else if(param == "ignore_hosts"  ) setting.ignore_hosts = entry[param].toString();
            else if(contains(Setting::types, param))
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
    if(autoconfig_url != rhs.autoconfig_url) return false;
    if(ignore_hosts.size() && rhs.ignore_hosts.size() && ignore_hosts != rhs.ignore_hosts) return false;

    for(auto const& [ type, uri ] : rhs.uris)
    {
        auto it = uris.find(type);
        if(it == uris.end() || it->second != uri) return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
QString Settings::match(const Setting&)
{
    QString name;

    return name;
}
