////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

////////////////////////////////////////////////////////////////////////////////
#include "profile.hpp"
#include "proxy.hpp"

#include <QGSettings>
#include <QObject>

#include <map>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
class settings : public QObject
{
    Q_OBJECT

public:
    explicit settings(QObject* parent = nullptr);

    using entry = profile::entry;
    entry current();

signals:
    void changed();

private:
    QGSettings proxy { proxy::schema_id };

    using type = QString;
    std::map<type, std::unique_ptr<QGSettings>> types;
};

////////////////////////////////////////////////////////////////////////////////
#endif
