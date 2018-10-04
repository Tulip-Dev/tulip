rem original script taken here: https://github.com/mypaint/libmypaint/blob/master/appveyor.bat
rem Matrix-driven Appveyor CI script for Tulip using GCC compiler provided by MSYS2
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Set the paths appropriately
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Upgrade the MSYS2 platform
bash -lc "pacman --noconfirm --sync --refresh --sysupgrade"

rem Install required tools
bash -lc "pacman --noconfirm -S --needed base-devel unzip mingw-w64-%MSYS2_ARCH%-toolchain"

rem Install the relevant native dependencies
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-freetype"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-glew"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-libpng"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-pcre2"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-qt5"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-quazip"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-qtwebkit"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-cppunit"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-ccache"

rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

rem Install Inetc plugin for NSIS
bash -lc "unzip bundlers/win/Inetc.zip -d\"C:\Program Files (x86)\NSIS\""

rem Build Tulip with Python 3, run its unit tests and package it
bash -lc "mkdir build"
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/build/install -DTULIP_BUILD_DOC=OFF -DTULIP_BUILD_TESTS=ON -DTULIP_USE_QT5=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON3_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 install"
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make runTests"
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make bundle"

rem if %errorlevel% neq 0 exit /b %errorlevel%

rem rem Install sphinx for Python 2
rem set PATH=%PYTHON2_HOME%;%PYTHON2_HOME%/Scripts;%PATH%
rem pip install sphinx

rem rem Build Tulip with Python 2, run its unit tests and package it
rem bash -lc "mkdir build"
rem bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/build/install -DTULIP_BUILD_DOC=OFF -DTULIP_BUILD_TESTS=ON -DTULIP_USE_QT5=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON2_HOME%/python.exe .."
rem if %errorlevel% neq 0 exit /b %errorlevel%
rem bash -lc "cd build && make -j4 install"
rem if %errorlevel% neq 0 exit /b %errorlevel%
rem bash -lc "cd build && make runTests"
rem if %errorlevel% neq 0 exit /b %errorlevel%
rem bash -lc "cd build && make bundle"
