# Decentralised
A 3D environment in a decentralised P2P blockchain.

http://decentralised-project.org

----

### Build on Windows

##### Install Visual Studio 2013 and Nov CTP Compiler Update

http://go.microsoft.com/fwlink/?LinkId=517284
http://www.microsoft.com/en-us/download/details.aspx?id=41151

##### Install SourceTree and clone repository

https://www.sourcetreeapp.com/

##### Install CMake

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

Open Decentralised.sln in /build folder, and build.

Open Libbitcoin project properties, and change compiler to Nov CTP.

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
