This directory contains configuration files for the cmake build system 

Requirements 
------------
- cmake version >= 2.4.4 see http://www.cmake.org 
- installed libxml2 or libexpat 

Building 
--------

unix
1. install cmake and libxml or libexpat 
2. get dbus sources 
3. mkdir dbus-build 
4. cd dbus-build 
5. cmake <dbus-src-root>/cmake or cmake -DDBUS_USE_EXPAT=on <dbus-src-root>/cmake in case libexpat should de used
5. make 
6. make install

win32-mingw
1. install cmake and libxml or libexpat in <ProgramDir>\gnuwin32
2. get dbus sources 
3. mkdir dbus-build 
4. cd dbus-build 
5. cmake -G "MinGW Makefiles" <dbus-src-root>/cmake
6. make 
7. make install

win32-msvc
1. install cmake and libxml or libexpat in <ProgramDir>\gnuwin32
2. get dbus sources 
3. mkdir dbus-build 
4. cd dbus-build 
5. cmake -G <msvc available target, see cmake --help for a list> <dbus-src-root>/cmake
6. make 
7. make install

A list of all available build options could be find in the file ../README.

