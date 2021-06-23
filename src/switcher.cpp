////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "profile.hpp"
#include "settings.hpp"

#include <QFile>
#include <QStandardPaths>
#include <QString>

#include <iostream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    if(argc == 2)
    {
        auto const path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0];

        QFile file { path + "/switcher.conf" };
        if(!file.open(QFile::ReadOnly))
        {
            auto err = "Can't open file " + file.fileName() + " - " + file.errorString();
            throw std::runtime_error { err.toStdString() };
        }

        auto entries = profile::read_from(file);

        QString name { argv[1] };
        auto pi = entries.find(name);
        if(pi == entries.end())
        {
            auto err = "Invalid profile: " + name;
            throw std::runtime_error { err.toStdString() };
        }

        settings{ }.switch_to(pi->second);

        return 0;
    }

    std::cout << "Usage: switcher <profile>\n" << std::endl;
    throw std::invalid_argument { "Invalid number of parameters" };
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
