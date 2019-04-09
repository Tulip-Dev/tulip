@echo off
set NSIS_PATH=%1
set TULIP_DIR=%2
set DEST_DIR=%3
if [%4] NEQ [] (
  if "%4" == "TRUE" (
    set DEBUG_MODE=%4
  ) else if NOT "%4" == "FALSE" (
    set OUT_FILE=%4
  )
)
set SRC_DIR=%cd%

echo 'Wiping destination directory'
rem In order to remove the contents of a non empty folder
rem we create an empty folder to clone in the first one using robocopy
rem the empty folder will be removed at the end
if NOT EXIST "%DEST_DIR%\\empty_folder" ( mkdir "%DEST_DIR%\\empty_folder" )
if EXIST "%DEST_DIR%\\files" (
  robocopy "%DEST_DIR%\\empty_folder" "%DEST_DIR%\\files" /MIR >nul 2>&1
) else ( mkdir "%DEST_DIR%\\files" )

echo 'Copying Tulip files'
xcopy "%TULIP_DIR%" "%DEST_DIR%\\files" /E /Q

if NOT "%DEBUG_MODE%" == "TRUE" (
echo 'Removing debug libs'
del /Q /F /S "%DEST_DIR%\\files\\bin\\Qt5*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\imageformats\\q*d.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\platforms\\q*d.pdb" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\styles\\q*d.dll" >nul 2>&1
del /Q /F /S "%DEST_DIR%\\files\\bin\\styles\\q*d.pdb" >nul 2>&1
)

echo 'Removing non dll files from lib directory'
del /Q /F /S "%DEST_DIR%\\files\\lib\\*.dll.a" >nul 2>&1
rem same trick with our empty folder
robocopy "%DEST_DIR%\\empty_folder" "%DEST_DIR%\\files\\lib\\cmake" /MIR >nul 2>&1
rmdir /Q /S "%DEST_DIR%\\files\\lib\\cmake" >nul 2>&1

echo 'Copying NSIS script and licence'

copy "%SRC_DIR%\\Tulip.nsi" "%DEST_DIR%\\"
copy "%SRC_DIR%\\COPYING.LESSER" "%DEST_DIR%\\"

echo 'Running NSIS installer generator'
cd "%DEST_DIR%"
set PATH=%NSIS_PATH%;%PATH%
if "%OUT_FILE%" == "" (
  makensis /V4 Tulip.nsi
) else (
  makensis /V4 /DOUT_FILE=%OUT_FILE% Tulip.nsi
)

echo 'NSIS installer generator completed !'

rem final removal
rmdir /Q /S "%DEST_DIR%\\empty_folder" >nul 2>&1
