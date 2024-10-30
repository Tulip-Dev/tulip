cd C:\Tulip5
PATH c:\msys64\ucrt64\bin;c:\msys64\usr\bin;c:\Windows\System32;%PATH%

echo on

rem first download nuget.exe needed to install mininaml version of python
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

rem Install supported version of python with nuget

rem Python 3.9.13
nuget.exe install python -Version 3.9.13 -ExcludeVersion -OutputDirectory .
mv python Python-39
Python-39\tools\python.exe -m pip install --upgrade pip
Python-39\tools\python.exe -m pip install --upgrade setuptools
Python-39\tools\python.exe -m pip install sip build delvewheel

rem Python 3.10.11
nuget.exe install python -Version 3.10.11 -ExcludeVersion -OutputDirectory .
mv python Python-310
Python-310\tools\python.exe -m pip install --upgrade pip
Python-310\tools\python.exe -m pip install sip build delvewheel

rem Python 3.11.9
nuget.exe install python -Version 3.11.9 -ExcludeVersion -OutputDirectory .
mv python Python-311
Python-311\tools\python.exe -m pip install --upgrade pip
Python-311\tools\python.exe -m pip install sip build delvewheel

rem Python 3.12.7
nuget.exe install python -Version 3.12.7 -ExcludeVersion -OutputDirectory .
mv python Python-312
Python-312\tools\python.exe -m pip install --upgrade pip
Python-312\tools\python.exe -m pip install sip build delvewheel

rem Python 3.13.0
nuget.exe install python -Version 3.13.0 -ExcludeVersion -OutputDirectory .
mv python Python-313
Python-313\tools\python.exe -m pip install --upgrade pip
Python-313\tools\python.exe -m pip install sip build delvewheel

mkdir wheels
mkdir build
rem cd build

rem wheel packages will be store in the wheels directory
set tulip_wheels_prefix="C:\Tulip5\wheels"
mkdir %tulip_wheels_prefix%

for /D %%G in ("C:\Tulip5\Python-*") do (
  setlocal EnableDelayedExpansion
  set pydir=%%G
  set pyver=!pydir:~17!
  if !pyver! GEQ 39 (
    set pyexe="%%G\tools/python.exe"
    PATH %%G\tools;%%G\Scripts;%PATH%
    rd /s /q build
    mkdir build
    cd build
    cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_COLOR_MAKEFILE=OFF -DPython_EXECUTABLE=!pyexe! -DPython_INCLUDE_DIRS=!pydir!/include -DTULIP_WHEELS_PREFIX=!tulip_wheels_prefix! -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DTULIP_USE_CCACHE=ON Z:
    if !ERRORLEVEL! NEQ 0 exit /B 1
    make -j4 wheel
    if !ERRORLEVEL! NEQ 0 exit /B 1
    set "FC=1"
    for /F %%F in ('dir /S /B %tulip_wheel_prefix%\*-cp!pyver!*.whl /O:-D') do (
      if !FC! == 1 (
        set /A FC-=1
        !pyexe! -m pip install %%F
        if !ERRORLEVEL! NEQ 0 exit /B 1
      !pyexe! -c "from tulip import tlp; from platform import python_version; str = 'Tulip ' + tlp.getTulipRelease() + ' successfully imported in Python ' + python_version(); print(str); l = tlp.getSizeAlgorithmPluginsList();exit(1) if not l else exit(0)"
        if !ERRORLEVEL! NEQ 0 exit /B 1
        !pyexe! -m pip uninstall -y tulip-python
        if !ERRORLEVEL! NEQ 0 exit /B 1
      )
    cd ..
    )
  )
)
