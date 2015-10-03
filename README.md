Eva
===
[![Join the chat at https://gitter.im/ieeeugrsb/Eva](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ieeeugrsb/Eva?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![Wall-E and Eva logo](http://ieee-ugr.org/wp-content/uploads/2014/03/logo-WandE-e1431951239314.png)

Source code repository of Eva project.

**Eva** is an *Unmanned Aerial Vehicule* capable of locating a known object on a controlled environment. Object location information is sent to [Wall-E](https://github.com/ieeeugrsb/Wall-E) autonomous robotic arm.

Project is composed by four teams:

* Structure
* Control
* Object recognition
* Communications (shared with Wall-E project)

## Compile and upload Arduino Sketches
In order to compile and upload the skeches for Arduino will be need:

* **Arduno Mega 2560**. They could work in other Arduinos after remapping the pins. You will need to edit *ano.ini* files with your board model.
* [**Arturo**](https://github.com/scottdarch/Arturo/). Fork of *ino*, it's a command-line program for compiling and uploading code to Arduino.

Then go the program folder (ie: *drone_control*) and run
``` bash
# For the first time you will need to download the dependencies too with:
git submodule update --init --recursive

ano build   # It will compile everything in 'src' dir.
ano upload  # It will upload the code to the Arduino board.
```

To interact with the Arduino program via serial port use the script *open_serial.sh* and pass as first and only argument the number of the port where Arduino is, ie: `./open_serial.sh 0` for */dev/ttyACM0* (default one).

The recommended text editor to continue with Arduino development is [Atom](https://github.com/atom/atom).

## Schematic Setup
To work with the drone schematic you will need the latest version of the gEDA suite. You can follow these steps to compile on UNIX systems:

``` bash
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
