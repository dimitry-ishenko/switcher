////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef GIO_HPP
#define GIO_HPP

////////////////////////////////////////////////////////////////////////////////
#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////
namespace proxy
{

constexpr auto schema_id = "org.gnome.system.proxy";

constexpr auto mode = "mode";
constexpr auto autoconfig_url = "autoconfig-url";
constexpr auto ignore_hosts = "ignore-hosts";

constexpr auto modes = { "none", "auto", "manual" };
constexpr auto types = { "http", "https", "ftp", "socks" };

constexpr auto host = "host";
constexpr auto port = "port";

}

////////////////////////////////////////////////////////////////////////////////
#endif
