rem original script taken here: https://github.com/mypaint/libmypaint/blob/master/appveyor.bat
rem Matrix-driven Appveyor CI script for Tulip using GCC compiler provided by MSYS2
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Check which type of Tulip build to perform based on the job number
rem   - odd number = core build
rem   - even number = complete build
rem We build Tulip in two phases (core build then complete build)
rem to avoid AppVeyor build timeouts. Object files generated during
rem the core build will be made available for the complete build
rem thanks to the use of ccache.
set /a TULIP_BUILD_CORE_ONLY = %APPVEYOR_JOB_NUMBER% %% 2
echo TULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY%

rem Set the paths appropriately
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem first manually install the new MSYS2 keyring from repo.msys2.org
rem see https://www.msys2.org/news/#2020-06-29-new-packagers
rem if repo.msys2.org is not online, use a mirror site
rem see https://github.com/msys2/MINGW-packages/issues/7084
bash -lc "pacman --noconfirm -U http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz; if [ \"$?\" != \"0\" ]; then echo '*** repo.msys2.org seems down, use mirror site instead ***'; touch '%cd%/use_mirror_site'; else echo '*** repo.msys2.org is online ***'; fi"

rem configure mirror site if needed
if exist use_mirror_site (
bash -lc "echo 'configure https://mirror.selfnet.de/msys2 as mirror site'; echo 'Server = https://mirror.selfnet.de/msys2/mingw/x86_64/' > /etc/pacman.d/mirrorlist.mingw64; echo 'Server = https://mirror.selfnet.de/msys2/mingw/i686/' > /etc/pacman.d/mirrorlist.mingw32; echo 'Server = https://mirror.selfnet.de/msys2/msys/$arch/' > /etc/pacman.d/mirrorlist.msys; pacman --noconfirm -U https://mirror.selfnet.de/msys2/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz;")

rem Update the MSYS2 platform
rem without --sysupgrade which fails
rem since MSYS2 version 20200517 is online
bash -lc "pacman --noconfirm --sync --refresh"
rem Update pacman
rem https://www.msys2.org/news/#2020-05-31-update-fails-with-could-not-open-file
bash -lc "pacman --noconfirm -Sydd pacman"

rem finally upgrade the platform
bash -lc "pacman --noconfirm -Syu"

rem as pacman has been updated configure mirror site if needed
if exist use_mirror_site (
bash -lc "echo 'Server = https://mirror.selfnet.de/msys2/mingw/x86_64/' > /etc/pacman.d/mirrorlist.mingw64; echo 'Server = https://mirror.selfnet.de/msys2/mingw/i686/' > /etc/pacman.d/mirrorlist.mingw32; echo 'Server = https://mirror.selfnet.de/msys2/msys/$arch/' > /etc/pacman.d/mirrorlist.msys")

rem display pacman version
bash -lc "pacman -V"

rem Install required tools
bash -lc "pacman --noconfirm -S --needed base-devel unzip"

rem Always install latest GCC toolchain in order to detect possible build failures
rem when its version evolves
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-toolchain"

rem Install the relevant native dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cmake"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-ccache"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cppunit"

set TULIP_BUILD_DOC=OFF

if "%TULIP_BUILD_CORE_ONLY%" == "0" (
  goto install_complete_tulip_build_dependencies
) else (
  goto tulip_build
)

:install_complete_tulip_build_dependencies
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-freetype"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-glew"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qt5"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-quazip"
bash -lc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-qtwebkit"

set TULIP_BUILD_DOC=ON
goto tulip_build


:tulip_build
rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

rem Install Inetc plugin for NSIS
bash -lc "unzip bundlers/win/Inetc.zip -d\"C:\Program Files (x86)\NSIS\""

rem Install sphinx for Python 3
set PATH=%PYTHON3_HOME%;%PYTHON3_HOME%/Scripts;%PATH%
pip install sphinx

rem Build Tulip with Python 3, run its unit tests and package it
bash -lc "mkdir build"
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=%APPVEYOR_BUILD_FOLDER%/build/install -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% -DTULIP_BUILD_DOC=%TULIP_BUILD_DOC% -DTULIP_BUILD_TESTS=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON3_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 install"
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make runTests"
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TULIP_BUILD_CORE_ONLY%" == "0" (
  bash -lc "cd build && make bundle"
)

rem Build Tulip without Python, and package it
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_NEED_RESPONSE=ON -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% -DTULIP_BUILD_DOC=%TULIP_BUILD_DOC% -DTULIP_BUILD_TESTS=ON -DTULIP_USE_CCACHE=ON -DTULIP_BUILD_PYTHON_COMPONENTS=OFF .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 install"
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TULIP_BUILD_CORE_ONLY%" == "0" (
  bash -lc "cd build && make bundle"
)
