# Wirus Mod Manager


| [English](README.md) | [Русский](README.ru.md) |
| -------------------- | ----------------------- |

Wirus Mod Manager is a lightweight mod manager based on a symlink system.  
Main features:
- Manage multiple isolated mod builds
- Combine modifications into presets
- Import and export builds
- Linux and Windows support

## Dependencies
WMM requires **Qt6** and **libarchive** to run.  
On **Windows**, all dependencies are shipped with the executable.  
On **Linux**, you need to install the required packages manually:

- Fedora: `sudo dnf install qt6-qtbase libarchive`  
- Debian: `sudo apt install qt6-base libarchive13`  
- Arch: `sudo pacman -S qt6-base libarchive`  

## Installation
Download the [release archive](https://github.com/Procionn/WMM/releases) for your system and unpack it.  

**Note:**  
Mods will be stored in the program’s directory. To avoid in-game stutters, it is recommended to install the program on the same drive as the game, or on another drive with comparable read speed.

## Building from source

### Linux
To build the project from source, you need the dev packages of dependencies:

- Fedora: `sudo dnf install cmake g++ make qt6-devel libarchive-devel`  
- Debian: `sudo apt install cmake g++ make qt6-dev libarchive-dev`  
- Arch: `sudo pacman -S cmake base-devel qt6-base libarchive`  

Then clone the repository:

`git clone https://github.com/Procionn/wmm.git`

And build it:  
`cmake -B "test" && cmake --build "test"`

### Windows:
Remove Windows
Install Linux
Follow the Linux instructions above

If the above steps do not convince you, it is strongly recommended to use QtCreator to build the project, as it will resolve most dependencies for you.

## License
This project is licensed under GPL-2.0.
See the LICENSE file for details.

In short — you are free to copy, distribute, and modify the project, but you may not distribute it under a closed-source license.

## third-party licenses
- [Qt 6](https://www.qt.io/) — GPL-3.0 
- [libarchive](https://www.libarchive.org/) — BSD 2-clause 

## Issues

Please report any problems or ideas in the [issues](https://github.com/Procionn/WMM/issues) section.
