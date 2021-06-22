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
namespace gio
{
namespace proxy
{

constexpr auto url = "org.gnome.system.proxy";

constexpr auto modes = { "none", "auto", "manual" };
constexpr auto types = { "http", "https", "ftp", "socks" };

}
}

////////////////////////////////////////////////////////////////////////////////
#endif
