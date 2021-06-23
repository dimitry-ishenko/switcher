# Proxy Switcher

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
$ ver=0.3
$ url=https://github.com/dimitry-ishenko/switcher/releases/download/v${ver}
$ wget ${url}/switcher_${ver}_amd64.deb
$ sudo apt install ./switcher_${ver}_amd64.deb
```

RaspberryPi:

```shell
$ ver=0.3
$ url=https://github.com/dimitry-ishenko/switcher/releases/download/v${ver}
$ wget ${url}/switcher_${ver}_armhf.deb
$ sudo apt install ./switcher_${ver}_armhf.deb
```

### From source

Stable version (requires [Qt](https://www.qt.io/) >= 5.9,
[qsettings-qt](https://gitlab.com/ubports/core/gsettings-qt) and 
[CMake](https://cmake.org/) >= 3.1):

```shell
$ ver=0.3
$ wget https://github.com/dimitry-ishenko/switcher/archive/v${ver}.tar.gz
$ tar xzf v${ver}.tar.gz
$ mkdir switcher-${ver}/build
$ cd switcher-${ver}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires [git](https://git-scm.com/), [Qt](https://www.qt.io/) >=
5.9, [qsettings-qt](https://gitlab.com/ubports/core/gsettings-qt) and 
[CMake](https://cmake.org/) >= 3.1):

```shell
$ git clone https://github.com/dimitry-ishenko/switcher.git
$ mkdir switcher/build
$ cd switcher/build
$ cmake ..
$ make
$ sudo make install
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
