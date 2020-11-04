rem Matrix-driven Appveyor CI script for Tulip using Visual Studio compiler

rem Check which type of Tulip build to perform based on the job number
rem   - odd number = core build
rem   - even number = complete build
rem We build Tulip in two phases (core build then complete build)
rem to avoid AppVeyor build timeouts. Object files generated during
rem the core build will be made available for the complete build
rem thanks to the use of clcache.
set /a TULIP_BUILD_CORE_ONLY = %APPVEYOR_JOB_NUMBER% %% 2
echo TULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY%

rem let's compile clcache in order to speedup incremental builds
cd C:/
set PATH=C:/Python35-x64;C:/Python35-x64/Scripts;%PATH%
pip install clcache
set CLCACHE_MSBUILD_CONF=/p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\Python35-x64\Scripts

rem create a directory to store Tulip dependencies
cd C:/ && md tulip_dependencies
cd C:/tulip_dependencies && md bin
cd C:/tulip_dependencies && md include
cd C:/tulip_dependencies && md lib

rem get, compile and install zlib
cd C:/tulip_dependencies
curl -LO https://zlib.net/zlib1211.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x zlib1211.zip
cd zlib-1.2.11
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install cppunit
cd C:/tulip_dependencies
git clone git://anongit.freedesktop.org/git/libreoffice/cppunit/
if %errorlevel% neq 0 exit /b %errorlevel%
cd cppunit/src
msbuild CppUnitLibraries2010.sln /m /clp:ErrorsOnly /p:Configuration=Release /p:Platform=%MSVC_PLATFORM% /p:PlatformToolset=%MSVC_PLATFORM_TOOLSET% %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%
xcopy C:\tulip_dependencies\cppunit\include C:\tulip_dependencies\include /S /Y
xcopy C:\tulip_dependencies\cppunit\lib C:\tulip_dependencies\lib /S /Y
move C:\tulip_dependencies\lib\cppunit_dll.dll C:\tulip_dependencies\bin\

if "%TULIP_BUILD_CORE_ONLY%" == "0" (
  goto install_complete_tulip_build_dependencies
) else {
  goto tulip_build
}


:install_complete_tulip_build_dependencies
rem get, compile and install freetype
cd C:/tulip_dependencies
curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-2.10.2.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x freetype-2.10.2.tar.gz -so | 7z x -aoa -si -ttar
cd freetype-2.10.2
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install glew
cd C:/tulip_dependencies
curl -L https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip/download -o glew-2.2.0.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x glew-2.2.0.zip
cd glew-2.2.0/build/cmake
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/tulip_dependencies" .
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

goto tulip_build


:tulip_build
rem we are good to go, let's compile and install Tulip now
cd %APPVEYOR_BUILD_FOLDER%
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INCLUDE_PATH="C:/tulip_dependencies/include" -DCMAKE_LIBRARY_PATH="C:/tulip_dependencies/lib;C:/tulip_dependencies/bin" -DCMAKE_PREFIX_PATH="%QT5_DIR%" -DPYTHON_EXECUTABLE="%PYTHON_EXECUTABLE%" -DTULIP_BUILD_CORE_ONLY=%TULIP_BUILD_CORE_ONLY% -DTULIP_BUILD_TESTS=ON -DTULIP_BUILD_DOC=OFF ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%
rem finally run Tulip tests
ctest --force-new-ctest-process --output-on-failure --build-config "Release"
