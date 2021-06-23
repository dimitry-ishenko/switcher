////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef PROFILE_HPP
#define PROFILE_HPP

////////////////////////////////////////////////////////////////////////////////
#include <QFile>
#include <QString>

#include <initializer_list>
#include <map>

////////////////////////////////////////////////////////////////////////////////
namespace profile
{

////////////////////////////////////////////////////////////////////////////////
constexpr auto mode = "mode";
constexpr auto autoconfig_url = "autoconfig_url";
constexpr auto ignore_hosts = "ignore_hosts";

constexpr auto modes = { "none", "auto", "manual" };
constexpr auto types = { "http", "https", "ftp", "socks" };

constexpr auto host = "host";
constexpr auto port = "port";

////////////////////////////////////////////////////////////////////////////////
struct entry
{
    QString mode;
    QString autoconfig_url;
    QString ignore_hosts;

    struct uri
    {
        QString host;
        int port;

        bool is_valid() const { return host.size() && port > 0; }
    };
    using type = QString;

    std::map<type, uri> types;
};

////////////////////////////////////////////////////////////////////////////////
using name = QString;
using entries = std::map<name, entry>;

entries read_from(QFile&);

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
