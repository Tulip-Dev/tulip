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
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-yajl"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-qhull"
bash -lc "pacman --noconfirm -S --needed --force mingw-w64-%MSYS2_ARCH%-ccache"

rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -lc "set pwd"
bash -lc "env"

rem Install needed wheel module for Python 2
set PATH=%PYTHON2_HOME%;%PYTHON2_HOME%/Scripts;%PATH%
pip install wheel

rem Build tulip-python wheel for Python 2
bash -lc "mkdir build"
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON2_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 wheel"
if %errorlevel% neq 0 exit /b %errorlevel%
rem Check that the produced wheel can be correctly imported
bash -lc "cd ./build/library/tulip-python/bindings/tulip-core/tulip_module/dist && pip install $(ls -t | head -1)"
python -c "import tulip"
if %errorlevel% neq 0 exit /b %errorlevel%

rem Install needed wheel module for Python 3
set PATH=%PYTHON3_HOME%;%PYTHON3_HOME%/Scripts;%PATH%
pip install wheel

rem Build tulip-python wheel for Python 3
bash -lc "mkdir build"
bash -lc "cd build && cmake -G \"MSYS Makefiles\" -DTULIP_ACTIVATE_PYTHON_WHEEL_TARGET=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_NEED_RESPONSE=ON -DTULIP_USE_CCACHE=ON -DPYTHON_EXECUTABLE=%PYTHON3_HOME%/python.exe .."
if %errorlevel% neq 0 exit /b %errorlevel%
bash -lc "cd build && make -j4 wheel"
rem Check that the produced wheel can be correctly imported
bash -lc "cd ./build/library/tulip-python/bindings/tulip-core/tulip_module/dist && pip install $(ls -t | head -1)"
python -c "import tulip"
