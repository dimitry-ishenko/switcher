////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

////////////////////////////////////////////////////////////////////////////////
#include <QFile>
#include <QString>

#include <map>
#include <set>

////////////////////////////////////////////////////////////////////////////////
using Mode = QString;
using Type = QString;

struct Uri
{
    QString host;
    int port = 0;

    bool is_valid() const { return host.size() && port > 0; }
    bool operator==(const Uri& rhs) const { return host == rhs.host && port == rhs.port; }
};

////////////////////////////////////////////////////////////////////////////////
struct Setting
{
    Mode mode;
    QString autoconfig_url, ignore_hosts;
    std::map<Type, Uri> uris;

    bool operator==(const Setting&) const;

    static const std::set<Mode> modes;
    static const std::set<Type> types;
};

////////////////////////////////////////////////////////////////////////////////
using Name = QString;

struct Settings : public std::map<Name, Setting>
{
    QString match(const Setting&);

    static Settings from(QFile&);
};

////////////////////////////////////////////////////////////////////////////////
#endif
