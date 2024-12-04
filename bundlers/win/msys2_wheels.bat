cd C:\Tulip5
PATH c:\msys64\ucrt64\bin;c:\msys64\usr\bin;c:\Windows\System32;%PATH%

echo on

rem first download nuget.exe needed to install minimal version of python
curl -o nuget.exe https://dist.nuget.org/win-x86-commandline/latest/nuget.exe

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

rem wheel packages will be stored in the wheels directory
rem build and wheels directory must already exist
set tulip_wheels_prefix="C:\Tulip5\wheels"
set pyexe=C:\Tulip5\python\tools\python.exe
PATH C:\Tulip5\python\tools;C:\Tulip5\python\Scripts;%PATH%


for %%G in (3.9.13 3.10.11 3.11.9 3.12.8 3.13.1) do (
rem Install supported version of python with nuget
  nuget.exe install python -Version %%G -ExcludeVersion -OutputDirectory .
  %pyexe% -m pip install --upgrade pip
  %pyexe% -m pip install sip build delvewheel
  setlocal EnableDelayedExpansion
  for /F "tokens=1,2 delims=." %%A in ('echo %%G') do set pymmver=%%A%%B
  cd build
  cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_COLOR_MAKEFILE=OFF -DPython_EXECUTABLE=%pyexe% -DPython_INCLUDE_DIRS=..\python/include -DTULIP_WHEELS_PREFIX=!tulip_wheels_prefix! -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_USE_CCACHE=ON Z:
  if !ERRORLEVEL! NEQ 0 exit /B 1
  make -j4 wheel
  if !ERRORLEVEL! NEQ 0 exit /B 1
  for /F %%F in ('dir /S /B %tulip_wheel_prefix%\*-cp!pymmver!*.whl /O:-D') do (
    %pyexe% -m pip install %%F
    if !ERRORLEVEL! NEQ 0 exit /B 1
    %pyexe% -c "from tulip import tlp; from platform import python_version; str = '===> Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str); l = tlp.getSizeAlgorithmPluginsList();exit(1) if not l else exit(0)"
    if !ERRORLEVEL! NEQ 0 exit /B 1
    %pyexe% -m pip uninstall -y tulip-python
    if !ERRORLEVEL! NEQ 0 exit /B 1
  )
  rem minimize contents removal for next wheel build
  del /q CMakeCache.txt
  rd /s /q library\tulip-python
  cd ..
  rem remove previous installed python
  rd /s /q C:\Tulip5\python
)
