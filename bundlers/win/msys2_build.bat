rem add the needed paths
PATH C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%

echo on

if not defined WORKSPACE (
  set WORKSPACE=%USERPROFILE%
)
echo WORKSPACE=%WORKSPACE%

if not defined TULIP_SRC (
  set TULIP_SRC=%WORKSPACE%
)
echo TULIP_SRC=%TULIP_SRC%

rem Install minimal Python 3 and pip if needed
if not defined PYTHON3_HOME (
  cd %WORKSPACE%
  curl -o nuget.exe https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
  nuget.exe install python -Version 3.13.0 -ExcludeVersion -OutputDirectory .
  curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
  python\tools\python.exe get-pip.py
  set PYTHON3_HOME=%WORKSPACE%\python\tools
)
echo PYTHON3_HOME=%PYTHON3_HOME%

rem upgrade MSYS2 platform according https://www.msys2.org/docs/ci/#appveyor
rem first is Core update, second is Normal update
if not defined NO_MSYS2_UPDATE (
  bash -lc "pacman --noconfirm -Syuu"
  pacman --noconfirm -Syuu
)

rem display pacman version
pacman -V

rem Install required tools
pacman --noconfirm -S --needed base-devel

rem Always install latest GCC toolchain in order to detect possible build failures
rem when its version evolves
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-toolchain

rem Install the relevant native dependencies
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-cmake
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-yajl
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-qhull
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-cppunit
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-zstd
pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-libzip

if not defined TULIP_BUILD_CORE_ONLY (
  set TULIP_BUILD_CORE_ONLY=OFF
)

if "%TULIP_BUILD_CORE_ONLY%" == "OFF" (
rem Install Inetc plugin for NSIS
  if not exist "C:\Program Files (x86)\NSIS\Contrib\Inetc" (
    pacman --noconfirm -S --needed unzip
    unzip %TULIP_SRC%\bundlers\win\Inetc.zip -d"C:\Program Files (x86)\NSIS\"
  )
  set TULIP_BUILD_DOC=-DTULIP_BUILD_DOC=ON
rem Install sphinx for Python 3
  set PATH=%PYTHON3_HOME%;%PYTHON3_HOME%/Scripts;%PATH%
  pip install sphinx
rem Install sip
  pip install sip
rem install_complete_tulip_build_dependencies
  pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-freetype
  pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-glew
  pacman --noconfirm -S --needed mingw-w64-ucrt-x86_64-qt5-base
) else (
  set TULIP_BUILD_DOC=""
)

rem create build dir
cd %WORKSPACE%
mkdir tulip_build
cd tulip_build

rem Build Tulip with Python 3, run its unit tests and package it
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_COLOR_MAKEFILE=OFF -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=./install -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% %TULIP_BUILD_DOC% -DTULIP_BUILD_TESTS=ON -DPython_EXECUTABLE=%PYTHON3_HOME%/python.exe %TULIP_SRC%
if %errorlevel% neq 0 exit /b %errorlevel%
make -j4 install
if %errorlevel% neq 0 exit /b %errorlevel%
make runTests
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TULIP_BUILD_CORE_ONLY%" == "OFF" (
  make bundle
rem package Tulip without python
  make no_python_bundle
)
