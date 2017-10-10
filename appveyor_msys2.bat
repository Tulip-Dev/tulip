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
bash -lc "pacman --noconfirm -S --needed base-devel mingw-w64-%MSYS2_ARCH%-toolchain"

rem Install the relevant native dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-freetype"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-glew"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-libpng"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-pcre2"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qt5"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-quazip"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qtwebkit"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-python3"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cppunit"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cmake"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ccache"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ninja"

rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

rem Build Tulip and run its unit tests
bash -lc "mkdir build"
bash -lc "cd build && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:/msys64/%MSYSTEM% -DTULIP_BUILD_TESTS=ON -DTULIP_USE_QT5=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=C:/msys64/%MSYSTEM%/bin/python3.exe -DTULIP_FIXUP_BUNDLE=OFF -DTULIP_PYTHON_SITE_INSTALL=ON .."
bash -lc "cd build && ninja -j4 install"
bash -lc "cd build && ninja runTests"
