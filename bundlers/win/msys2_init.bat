rem This scripts aims to perform automatic installation
rem of msys2 latest version
echo on

rem Download https://github.com/msys2/msys2-installer/releases/latest page
curl -L https://www.msys2.org -o msys2_latest

rem Extract url of latest msys2 install
PowerShell -Command "Select-String \"Download the installer\" msys2_latest > msys2_exe_line"
PowerShell -Command "Select-String \"https\" msys2_exe_line | Foreach-Object -process {$_.Line.Split('\"')[1]} > msys2_exe_url"
set /P MSYS2_EXE_URL= < msys2_exe_url

rem Download msys2.exe
curl -L "https://github.com%MSYS2_EXE_URL%" -o msys2.exe

rem uninstall old version if any
if exist c:\msys64\uninstall.exe c:\msys64\uninstall.exe -c pr

rem perform installation
.\msys2.exe -c -t c:/msys64 install

rem delete temporary files
del .\msys2*
