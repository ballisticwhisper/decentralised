# Decentralised
A 3D environment in a decentralised P2P blockchain.

http://decentralised-project.org

----

### Build on Windows

##### Install Visual Studio 2013

http://go.microsoft.com/fwlink/?LinkId=517284

##### Install SourceTree and clone repository

https://www.sourcetreeapp.com/

##### Install CMake

https://cmake.org/download/

Make sure cmake is in your PATH, and reboot after installation.

##### Create Visual Studio solution

Open a console and cd to the root folder ...

```
mkdir build
cd build
cmake .. -G "Visual Studio 12"
```

----

### Build on Linux

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

##### Install CMake

```
sudo apt-get install cmake
```

