rem Matrix-driven Appveyor CI script for Tulip using Visual Studio compiler

rem let's compile clcache in order to speedup incremental builds
cd C:/
git clone https://github.com/frerich/clcache.git
set PATH=C:/Python35-x64;C:/Python35-x64/Scripts;%PATH%
pip install pyinstaller --upgrade
cd clcache
pyinstaller pyinstaller/clcache_main.py -n clcache
set PATH=C:/clcache/dist/clcache;%PATH%

rem create a directory to store Tulip dependencies
cd C:/ && md tulip_dependencies 

rem get, compile and install freetype
cd C:/tulip_dependencies
curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-2.8.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x freetype-2.8.tar.gz -so | 7z x -aoa -si -ttar
cd freetype-2.8
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install zlib
cd C:/tulip_dependencies
curl -LO https://zlib.net/zlib1211.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x zlib1211.zip
cd zlib-1.2.11
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install libpng
cd C:/tulip_dependencies
curl -LO https://sourceforge.mirrorservice.org/l/li/libpng/libpng16/1.6.35/libpng-1.6.35.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x libpng-1.6.35.tar.gz -so | 7z x -aoa -si -ttar
cd libpng-1.6.35
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INCLUDE_PATH="C:/tulip_dependencies/include" -DCMAKE_LIBRARY_PATH="C:/tulip_dependencies/lib" -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /clp:ErrorsOnly /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install glew
cd C:/tulip_dependencies
curl -LO https://sourceforge.mirrorservice.org/g/gl/glew/glew/2.1.0/glew-2.1.0.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x glew-2.1.0.zip
cd glew-2.1.0/build/cmake
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" .
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install libjpeg
cd C:/tulip_dependencies
curl -LO https://sourceforge.mirrorservice.org/l/li/libjpeg-turbo/1.5.2/libjpeg-turbo-1.5.2.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x libjpeg-turbo-1.5.2.tar.gz -so | 7z x -aoa -si -ttar
cd libjpeg-turbo-1.5.2
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DWITH_SIMD=OFF -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install cppunit
cd C:/tulip_dependencies
git clone git://anongit.freedesktop.org/git/libreoffice/cppunit/
if %errorlevel% neq 0 exit /b %errorlevel%
cd cppunit/src
msbuild CppUnitLibraries2010.sln /m /clp:ErrorsOnly /p:Configuration=Release /p:Platform=%MSVC_PLATFORM% /p:PlatformToolset=%MSVC_PLATFORM_TOOLSET% /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%
xcopy C:\tulip_dependencies\cppunit\include C:\tulip_dependencies\include /S /Y
xcopy C:\tulip_dependencies\cppunit\lib C:\tulip_dependencies\lib /S /Y
move C:\tulip_dependencies\lib\cppunit_dll.dll C:\tulip_dependencies\bin\

rem we are good to go, let's compile and install Tulip now
cd %APPVEYOR_BUILD_FOLDER%
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INCLUDE_PATH="C:/tulip_dependencies/include" -DCMAKE_LIBRARY_PATH="C:/tulip_dependencies/lib;C:/tulip_dependencies/bin" -DCMAKE_PREFIX_PATH="%QT5_DIR%" -DTULIP_USE_QT5=ON -DPYTHON_EXECUTABLE="%PYTHON_EXECUTABLE%" -DTULIP_BUILD_TESTS=ON ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
if %errorlevel% neq 0 exit /b %errorlevel%
rem finally run Tulip tests
ctest --force-new-ctest-process --output-on-failure --build-config "Release"

rem build and test tulip-core-wheel for all Python versions (> 3.4) installed
if %MSVC_PLATFORM% == "x64" (set pysuffix=3?-x64) else (set pysuffix=3?)
for /D %%P in ("c:\Python%pysuffix%") do (
  setlocal EnableDelayedExpansion
  set pdir=%%P
  set pminor=!pdir:~10,1!
  if !pminor! gtr 4 (
    set pyexe=%%P/python.exe
    cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INCLUDE_PATH="C:/tulip_dependencies/include" -DCMAKE_LIBRARY_PATH="C:/tulip_dependencies/lib;C:/tulip_dependencies/bin" -DPYTHON_EXECUTABLE=!pyexe! -DTULIP_BUILD_CORE_ONLY=ON -DTULIP_ACTIVATE_PYTHON_WHEELS_TARGETS=ON ..
    if %errorlevel% neq 0 exit /b %errorlevel%
    msbuild tulip-core-wheel.vcxproj /m /p:Configuration=Release /p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\clcache\dist\clcache
    if %errorlevel% neq 0 exit /b %errorlevel%
    set "FC=1"
    for /F %%F in ('dir /S /B library\tulip-python\bindings\tulip-core\tulip_module\dist\*.whl /O:-D') do (
      if !FC! == 1 (
        !pyexe! -m pip install %%F
        !pyexe! -c "import tulip"
      )
      set /A FC-=1
    )
  )
  endlocal
)
