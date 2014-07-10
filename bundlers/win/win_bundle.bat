@echo off
set NSIS_PATH=%1
set TULIP_DIR=%2
set DEST_DIR=%3
set DEBUG_MODE=%4
set SRC_DIR=%cd%

echo 'Wiping destination directory'
del /Q /F /S "%DEST_DIR%\\files" >nul 2>&1
rd /S /Q "%DEST_DIR%\\files" >nul 2>&1
mkdir "%DEST_DIR%\\files"

echo 'Copying Tulip files'
xcopy "%TULIP_DIR%" "%DEST_DIR%\\files" /E /Q

if NOT "%DEBUG_MODE%" == "TRUE" (
echo 'Removing debug libs'
del /Q /F /S "%DEST_DIR%\\files\\bin\\*d4.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\Qt5*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d4.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\libq*.a" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d4.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\q*d.pdb" >nul 2>&1
)
echo 'Removing shared libs archives'
del /Q /F /S "%DEST_DIR%\\files\\lib\\lib*.dll.a" >nul 2>&1

echo 'Copying NSIS script and licence'
copy "%SRC_DIR%\\Tulip.nsi" "%DEST_DIR%\\"
copy "%SRC_DIR%\\COPYING" "%DEST_DIR%\\"

echo 'Running NSIS installer generator'
cd "%DEST_DIR%"
set PATH=%NSIS_PATH%;%PATH%
makensis Tulip.nsi

echo 'NSIS installer generator completed !'

del /Q /F /S "%DEST_DIR%\\files" >nul 2>&1
rd /S /Q "%DEST_DIR%\\files" >nul 2>&1
