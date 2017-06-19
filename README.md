# QGroundControl Ground Control Station
## Open Source Micro Air Vehicle Ground Control Station

[![Releases](https://img.shields.io/github/release/mavlink/QGroundControl.svg)](https://github.com/mavlink/QGroundControl/releases)
[![Travis Build Status](https://travis-ci.org/mavlink/qgroundcontrol.svg?branch=master)](https://travis-ci.org/mavlink/qgroundcontrol)
[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/crxcm4qayejuvh6c/branch/master?svg=true)](https://ci.appveyor.com/project/mavlink/qgroundcontrol)

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mavlink/qgroundcontrol?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

The license terms are set in the COPYING.md file.

* Project:
<http://qgroundcontrol.org>

* Files:
<http://github.com/mavlink/qgroundcontrol>

* Credits:
<http://qgroundcontrol.org/credits>


## Obtaining source code
Source code for QGroundControl is kept on GitHub: https://github.com/shadow1runner/qgroundcontrol.
```
git clone --recursive https://github.com/shadow1runner/qgroundcontrol.git
```
Each time you pull new source to your repository you should run `git submodule update` to get the latest submodules as well. Since QGroundControl uses submodules, using the zip file for source download will not work. You must use git.

### User Manual
https://donlakeflyer.gitbooks.io/qgroundcontrol-user-guide/content/

### Supported Builds

#### Native Builds
QGroundControl builds are supported for OSX, Linux, Windows, iOS and Android. QGroundControl uses [Qt](http://www.qt.io) as its cross-platform support library and uses [QtCreator](http://doc.qt.io/qtcreator/index.html) as its default build environment.
* OSX: OSX 10.7 or higher, 64 bit, clang compiler (IMPORTANT: XCode 8 requires a workaround described below)
* Ubuntu: 64 bit, gcc compiler
* Windows: Vista or higher, 32 bit, [Visual Studio 2013 compiler](http://www.visualstudio.com/downloads/download-visual-studio-vs#d-express-windows-desktop)
* iOS: 8.0 and higher
* Android: Jelly Bean (4.1) and higher
* Qt version: 5.5.1 ONLY

###### Install QT
You need to install Qt as described below instead of using pre-built packages from say, a Linux distribution because QGroundControl needs access to private Qt headers.
* Download the [Qt installer](http://www.qt.io/download-open-source)
    * Make sure to install Qt version **5.5.1** NOT 5.4.x, 5.6.x, 5.7.x, etc.  
    * Ubuntu: Set the downloaded file to executable using:`chmod +x`. Install to default location for use with ./qgroundcontrol-start.sh. If you install Qt to a non-default location you will need to modify qgroundcontrol-start.sh in order to run downloaded builds.
    * Windows: Default installer not quite correct, use [this](http://download.qt.io/official_releases/qt/5.5/5.5.1/qt-opensource-windows-x86-msvc2013-5.5.1.exe) instead

###### Install additional packages:
* Ubuntu: sudo apt-get install espeak libespeak-dev libudev-dev libsdl2-dev
* Fedora: sudo dnf install espeak espeak-devel SDL2-devel SDL2 systemd-devel
* Arch Linux: pacman -Sy espeak
* Windows: [USB Driver](http://www.pixhawk.org/firmware/downloads) to connect to Pixhawk/PX4Flow/3DR Radio
* Android: [Qt Android Setup](http://doc.qt.io/qt-5/androidgs.html)

###### Install 3rd party dependencies for Obstacle Avoidance
* OpenCV and OpenCV contrib
	+ `git clone https://github.com/opencv/opencv.git`
	+ `cd opencv`
	+ `git clone https://github.com/opencv/opencv_contrib.git`
	+ `git checkout 177aef0` <- last build I tested it with (OpenCV 3.1.0-dev);
	+ Install package dependencies to build from source listed [here](http://docs.opencv.org/3.0-beta/doc/tutorials/introduction/linux_install/linux_install.html)
	+ run CMake:
		- make sure to use `WITH_LIBV4L` option
		- make sure to specify the `OPENCV_EXTRA_MODULES_PATH` and let it point to OpenCV contrib
	+ run `make -j8; sudo make install`
	+ Problems I ran into:
		- [VIDIOC_QUERYCTRL: Inappropriate ioctl for device](https://github.com/opencv/opencv/issues/6157)
* Compile boost from source by following [this guide](http://www.boost.org/doc/libs/1_62_0/more/getting_started/unix-variants.html#easy-build-and-install) (last version tested: 1.62)

###### Building using Qt Creator

* Launch Qt Creator and open the `qgroundcontrol.pro` project.
* Select the appropriate kit for your needs:
    * OSX: Desktop Qt 5.5.1 clang 64 bit
    * Ubuntu: Desktop Qt 5.5.1 GCC bit
    * Windows: Desktop Qt 5.5.1 MSVC2013 32bit
    * Android: Android for armeabi-v7a (GCC 4.9, Qt 5.5.1)
* Note: iOS builds must be built using xCode: http://doc.qt.io/qt-5/ios-support.html. Use Qt Creator to generate the XCode project (*Run Qmake* from the context menu).

###### Configuring QGroundControl
* To get up to speed with `QGroundControl.ini`, I've provided configuration files for obstacle detection here:
	+ `git clone https://shadow1runner@bitbucket.org/shadow1runner/uavobstacledetectionconfig.git`


### Video Streaming
Check the [Video Streaming](https://github.com/mavlink/qgroundcontrol/tree/master/src/VideoStreaming) directory for further instructions.
