rem add the needed paths
PATH C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%

if not defined WORKSPACE (
  set WORKSPACE=%USERPROFILE%
)

rem Install minimal Python 3 and pip if needed
if not defined PYTHON3_HOME (
  cd %WORKSPACE%
  wget https://aka.ms/nugetclidl -O nuget.exe
  nuget.exe install python -Version 3.9.7 -ExcludeVersion -OutputDirectory .
  curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
  cp python\tools\python.exe python\tools\python3.exe
  python\tools\python3.exe get-pip.py
  set PYTHON3_HOME=%WORKSPACE%\python\tools
)
rem Install sphinx for Python 3
echo PYTHON3_HOME=%PYTHON3_HOME%
set PATH %PYTHON3_HOME%;%PYTHON3_HOME%\Scripts;%PATH%
%PYTHON3_HOME%\python3.exe -m pip install sphinx==1.7.9 --trusted-host pypi.org --trusted-host files.pythonhosted.org

rem upgrade MSYS2 platform according https://www.msys2.org/docs/ci/#appveyor
rem first is Core update, second is Normal update
bash -lc "pacman --noconfirm -Syuu"
bash -lc "pacman --noconfirm -Syuu"

rem display pacman version
pacman -V

rem Install required tools
pacman --noconfirm -S --needed base-devel unzip

rem Always install latest GCC toolchain in order to detect possible build failures
rem when its version evolves
pacman --noconfirm -S --needed mingw-w64-x86_64-toolchain

rem Install the relevant native dependencies
pacman --noconfirm -S --needed mingw-w64-x86_64-cmake
pacman --noconfirm -S --needed mingw-w64-x86_64-ccache
pacman --noconfirm -S --needed mingw-w64-x86_64-yajl
pacman --noconfirm -S --needed mingw-w64-x86_64-qhull
pacman --noconfirm -S --needed mingw-w64-x86_64-cppunit

rem Install Inetc plugin for NSIS
if not exist "C:\Program Files (x86)\NSIS\Contrib\Inetc" (
  unzip %TULIP_SRC%/bundlers/win/Inetc.zip -d"C:\Program Files (x86)\NSIS\"
)

if not defined TULIP_BUILD_CORE_ONLY (
  set TULIP_BUILD_CORE_ONLY=OFF
)

if "%TULIP_BUILD_CORE_ONLY%" == "OFF" (
  set TULIP_BUILD_DOC=-DTULIP_BUILD_DOC=ON
  goto install_complete_tulip_build_dependencies
) else (
  set TULIP_BUILD_DOC=""
  goto tulip_build
)

:install_complete_tulip_build_dependencies
pacman --noconfirm -S --needed mingw-w64-x86_64-freetype
pacman --noconfirm -S --needed mingw-w64-x86_64-glew
pacman --noconfirm -S --needed mingw-w64-x86_64-qt5
pacman --noconfirm -S --needed mingw-w64-x86_64-quazip
pacman --noconfirm -S --needed mingw-w64-x86_64-qtwebkit

:tulip_build
rem create build dir
set CHERE_INVOKING=yes
cd %WORKSPACE%
mkdir tulip_build
cd tulip_build

rem Build Tulip with Python 3, run its unit tests and package it
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_COLOR_MAKEFILE=OFF -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=./install -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% %TULIP_BUILD_DOC% -DTULIP_BUILD_TESTS=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON3_HOME%/python3.exe %TULIP_SRC%
if %errorlevel% neq 0 exit /b %errorlevel%
make -j4 install
if %errorlevel% neq 0 exit /b %errorlevel%
make runTests
if %errorlevel% neq 0 exit /b %errorlevel%

if "%TULIP_BUILD_CORE_ONLY%" == "OFF" (
  make bundle
)

rem Build Tulip without Python, and package it
if "%TULIP_BUILD_CORE_ONLY%" == "OFF" (
  cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_COLOR_MAKEFILE=OFF -DCMAKE_NEED_RESPONSE=ON -DCMAKE_INSTALL_PREFIX=./install -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% %TULIP_BUILD_DOC% -DTULIP_BUILD_TESTS=ON -DTULIP_USE_CCACHE=ON -DTULIP_BUILD_PYTHON_COMPONENTS=OFF %TULIP_SRC%
  if %errorlevel% neq 0 exit /b %errorlevel%
  make -j4 install
  if %errorlevel% neq 0 exit /b %errorlevel%
  make bundle
)
