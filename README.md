Eva
===
[![Join the chat at https://gitter.im/ieeeugrsb/Eva](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ieeeugrsb/Eva?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![Wall-E and Eva logo](http://ieee-ugr.org/wp-content/uploads/2014/03/logo-WandE-e1431951239314.png)

Source code repository of Eva project.

Eva is an Unmanned Aerial Vehicule capable of locating a known object on a controlled environment. Object location information is sent to [Wall-E](https://github.com/ieeeugrsb/Wall-E) autonomous robotic arm.

Project is composed by four teams:

* Structure
* Control
* Object recognition
* Communications (shared with Wall-E project)


## Schematic Setup
To work with the drone schematic you will need the latest version of the gEDA suite. You can follow these steps to compile on UNIX systems:

```
mkdir geda && cd geda

git clone git://git.geda-project.org/geda-gaf.git
cd geda-gaf
./autogen.sh
./configure --prefix=/opt/geda
make
sudo make install

git clone git://git.geda-project.org/pcb.git
cd pcb
./autogen.sh
./configure --prefix=/opt/geda
make
sudo make install
```

Then create or edit the file *~/.gEDA/gafrc* to add the new symbols:
```
(component-library "<project_path>/schematics/sym")
```

## External resources
* [Arduino gEDA Schematic Design](https://github.com/miloh/arduino-templates-gaf) by miloh.
* [Git hooks for gEDA](https://github.com/BenBergman/.git_hooks) by Benjamin Bergman.
