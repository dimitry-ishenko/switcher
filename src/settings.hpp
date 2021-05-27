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
struct Uri
{
    QString host;
    int port = 0;

    bool is_valid() const { return host.size() && port > 0; }

    bool operator==(const Uri& rhs) const { return host == rhs.host && port == rhs.port; }
    bool operator!=(const Uri& rhs) const { return !(*this == rhs); }
};

////////////////////////////////////////////////////////////////////////////////
struct Setting
{
    QString mode;
    QString autoconfig_url;
    QString ignore_hosts;
    std::map<QString, Uri> uris;

    static constexpr auto types = { "http", "https", "ftp", "socks" };

    bool operator==(const Setting&) const;
    bool operator!=(const Setting& rhs) const { return !((*this) == rhs); }
};

////////////////////////////////////////////////////////////////////////////////
struct Settings : public std::map<QString, Setting>
{
    static Settings from_file(const QString&);
    QString match(const Setting&);
};

////////////////////////////////////////////////////////////////////////////////
#endif
