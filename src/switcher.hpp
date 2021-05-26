////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SWITCHER_HPP
#define SWITCHER_HPP

////////////////////////////////////////////////////////////////////////////////
#include <QObject>
#include <QString>
#include <map>

////////////////////////////////////////////////////////////////////////////////
class Switcher : public QObject
{
    Q_OBJECT

public:
    explicit Switcher(const QString& name, QObject* parent = nullptr);

    void switch_to(const QString&);

private:
    struct Uri
    {
        QString host;
        int port = 0;
    };

    struct Proxy
    {
        QString mode;
        QString autoconfig_url;
        QString ignore_hosts;
        std::map<QString, Uri> uris;
    };

    std::map<QString, Proxy> settings;
};

////////////////////////////////////////////////////////////////////////////////
#endif
