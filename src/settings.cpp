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
                setting.uris[param].host = values["host"].toString();
                setting.uris[param].port = values["port"].toInt(0);
            }
        }
    }

    return settings;
}

////////////////////////////////////////////////////////////////////////////////
QString Settings::match(const Setting&)
{
    QString name;

    return name;
}
