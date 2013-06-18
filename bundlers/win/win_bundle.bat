@echo off
set TULIP_DIR=%1
set DEST_DIR=%2
set QT_PLUGINS_DIR=%3
set SRC_DIR=%cd%

echo 'Wiping destination directory'
del /Q /F /S "%DEST_DIR%\\files" >nul 2>&1
rd /S /Q "%DEST_DIR%\\files" >nul 2>&1
mkdir "%DEST_DIR%\\files"

echo 'Copying Tulip files'
xcopy "%TULIP_DIR%" "%DEST_DIR%\\files" /E /Q
mkdir "%DEST_DIR%\\files\\bin\\imageformats"
xcopy "%QT_PLUGINS_DIR%"\\imageformats "%DEST_DIR%\\files\\bin\\imageformats" /E /Q

del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d4.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d4.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.pdb" >nul 2>&1

echo 'Copying NSIS script and licence'
copy "%SRC_DIR%\\Tulip.nsi" "%DEST_DIR%\\"
copy "%SRC_DIR%\\COPYING" "%DEST_DIR%\\"

echo 'Running NSIS installer generator'
cd "%DEST_DIR%"
makensis Tulip.nsi

echo 'NSIS installer generator completed !'

del /Q /F /S "%DEST_DIR%\\files" >nul 2>&1
rd /S /Q "%DEST_DIR%\\files" >nul 2>&1