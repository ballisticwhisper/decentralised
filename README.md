# Decentralised 
A 3D environment in a decentralised P2P blockchain.

http://decentralised-project.org

----

### Build on Windows

##### Prerequisites

* [Visual Studio 2013](http://go.microsoft.com/fwlink/?LinkId=517284)
* [Visual C++ Compiler November 2013 CTP](http://www.microsoft.com/en-us/download/details.aspx?id=41151)
* [Netwide Assembler](http://nasm.sourceforge.net/)
* [CMake 3.4+](https://cmake.org/download/)
* [Python](https://www.python.org/)
* [ActivePerl](http://www.activestate.com/activeperl/downloads)
* [wxWidgets](https://www.wxwidgets.org/downloads/)

Make sure CMake, Python and Perl are in your PATH, and reboot after installation.

Make sure submodules are downloaded in /dependencies

##### Create Visual Studio solution

Open a console and cd to the root folder ...

```
mkdir build
cd build
python ../configure-windows.py
```

##### Build boost

Open a console and cd to the dependencies/boost folder

```
bootstrap
.\b2
```

##### Build openssl

Open a Visual Studio console and cd to the dependencies/openssl folder

```
mkdir [path-to-root-folder]\build\openssl
perl Configure VC-WIN32 enable-static-engine --prefix=[path-to-root-folder]\\build\\openssl
ms\do_nasm
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
```

##### Build curl

Open a Visual Studio console and cd to the dependencies/curl/winbuild folder

```
nmake /f Makefile.vc mode=static VC=12 
```

##### Build ogredeps

Open cmake-gui (if cmake is in your path type cmake-gui in console)

In "Where is the source code" box, enter:
```
[path-to-root-folder]/dependencies/ogredeps
```
In "Where to build the binaries" box, enter:
```
[path-to-root-folder]/build/ogredeps
```
Hit "Configure" button, select Visual Studio 2013, and "Finish".

Hit "Generate" button

Open [path-to-root-folder]/build/ogredeps/OGREDEPS.sln with Visual Studio, and build (In Release AND Debug).


##### Build ogre

Open cmake-gui (if cmake is in your path type cmake-gui in console)

In "Where is the source code" box, enter:
```
[path-to-root-folder]/dependencies/ogre
```
In "Where to build the binaries" box, enter:
```
[path-to-root-folder]/build/ogre
```
Hit "Configure" button, select Visual Studio 2013, and "Finish".



##### Build decentralised

Open Decentralised.sln in /build folder.

Set 'decentralised' as the start up project.

Set the debug working directory on 'decentralised' to ../../Debug

Build & Execute

----
