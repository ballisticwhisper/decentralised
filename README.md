# Decentralised
A 3D environment in a decentralised P2P blockchain.

[![Build Status](https://travis-ci.org/ballisticwhisper/decentralised.svg)](https://travis-ci.org/ballisticwhisper/decentralised)

http://decentralised-project.org

----

### Build on Windows

##### Install Visual Studio 2013 and Nov CTP Compiler Update

http://go.microsoft.com/fwlink/?LinkId=517284

http://www.microsoft.com/en-us/download/details.aspx?id=41151

##### Install SourceTree and clone repository

https://www.sourcetreeapp.com/
Also make sure submodules are downloaded in /dependencies

##### Install CMake 3.4+

https://cmake.org/download/

Make sure cmake is in your PATH, and reboot after installation.

##### Build boost

Open a console and cd to the dependencies/boost folder

```
bootstrap
.\b2
```

##### Create Visual Studio solution

Open a console and cd to the root folder ...

```
mkdir build
cd build
cmake .. -G "Visual Studio 12"
```

Open Decentralised.sln in /build folder.

Can't find a way to make CMake set Platform Toolset for individual projects.
Ref: https://cmake.org/Bug/view.php?id=13961#c32409

So you need to set the following projects in Visual Studio to use CTP_Nov2013 toolset.

* decentralised
* decentralised_core
* libbitcoin
* secp256k1

Build

----

### Build on Linux (to be updated)

##### Install Git

```
sudo apt-get install git
```

##### Clone repository and submodules

```
git clone https://github.com/ballisticwhisper/decentralised.git dc
git submodule init
git submodule update
```

##### Install CMake and OpenGL

```
sudo apt-get install cmake
sudo apt-get install freeglut3 freeglut3-dev freeglut3-dbg
```

##### Setup the build folder, and build

Open a terminal and cd to the root folder ...

```
mkdir build
cd build
cmake .. -G "CodeBlocks - Unix Makefiles"
make
```
