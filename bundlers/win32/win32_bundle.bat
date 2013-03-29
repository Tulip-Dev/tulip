@echo off
set TULIP_DIR=%1
set DEST_DIR=%2
set SRC_DIR=%cd%

echo 'Wiping destination directory'
del /Q /F /S %DEST_DIR%
mkdir "%DEST_DIR%\\files\\lib"
mkdir "%DEST_DIR%\\files\\bin"
mkdir "%DEST_DIR%\\files\\include"

echo 'Copying tulip files'
xcopy "%TULIP_DIR%" "%DEST_DIR%\\files" /E /Q
echo 'Deleting tulip unneeded libraries'
del "%DEST_DIR%\\files\\lib\*.dll.a" /Q
del "%DEST_DIR%\\files\\lib\tulip\*.dll.a" /Q
del "%DEST_DIR%\\files\\lib\tulip\glyph\*.dll.a" /Q
del "%DEST_DIR%\\files\\lib\tulip\interactor\*.dll.a" /Q
del "%DEST_DIR%\\files\\lib\tulip\perspective\*.dll.a" /Q
del "%DEST_DIR%\\files\\lib\tulip\view\*.dll.a" /Q

echo 'Copying NSIS script'
copy "%SRC_DIR%\\Tulip.nsi" "%DEST_DIR%\\"
copy "%SRC_DIR%\\..\\..\\COPYING" "%DEST_DIR%\\"

echo 'Running NSIS generator'
cd %DEST_DIR%
makensis Tulip.nsi
cd %SRC_DIR%
