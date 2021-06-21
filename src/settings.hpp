////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

////////////////////////////////////////////////////////////////////////////////
#include <QString>
#include <map>

////////////////////////////////////////////////////////////////////////////////
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
    QString mode;
    QString autoconfig_url;
    QString ignore_hosts;
    std::map<Type, Uri> uris;

    static constexpr auto modes = { "none", "manual", "auto" };
    static constexpr auto types = { "http", "https", "ftp", "socks" };

    bool operator==(const Setting&) const;
};

////////////////////////////////////////////////////////////////////////////////
using Name = QString;

struct Settings : public std::map<Name, Setting>
{
    static Settings from_file(const QString&);
    QString match(const Setting&);
};

////////////////////////////////////////////////////////////////////////////////
#endif
