@echo off
set NSIS_PATH=%1
set TULIP_DIR=%2
set DEST_DIR=%3
set BUNDLE_TYPE=%4
if [%5] NEQ [] (
  if "%5" == "TRUE" (
    set DEBUG_MODE=%5
  ) else if NOT "%5" == "FALSE" (
    set OUT_FILE=%5
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

echo 'Removing include directory'
rmdir /Q /S "%DEST_DIR%\\files\\include" >nul 2>&1

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

if "%BUNDLE_TYPE%" == "WITHOUT_PYTHON" (
echo 'Removing tulip-python lib'
del /Q /F /S "%DEST_DIR%\\files\\lib\\tulip\\libtulip-python*.dll" >nul 2>&1
echo 'Removing lib/tulip/python directory'
rmdir /Q /S "%DEST_DIR%\\files\\lib\\tulip\\python" >nul 2>&1
echo 'Removing python doc'
rmdir /Q /S "%DEST_DIR%\\files\\share\\doc\\Tulip\\tulip-python" >nul 2>&1
)

echo 'Removing non dll files from lib directory'
del /Q /F /S "%DEST_DIR%\\files\\lib\\*.dll.a" >nul 2>&1
rem same trick with our empty folder
robocopy "%DEST_DIR%\\empty_folder" "%DEST_DIR%\\files\\lib\\cmake" /MIR >nul 2>&1
rmdir /Q /S "%DEST_DIR%\\files\\lib\\cmake" >nul 2>&1

echo 'Copying NSIS script and licence'
if "%BUNDLE_TYPE%" == "WITHOUT_PYTHON" (
set TULIP_NSI=Tulip_no_python.nsi
) else (set TULIP_NSI=Tulip.nsi)

copy "%SRC_DIR%\\%TULIP_NSI%" "%DEST_DIR%\\"
copy "%SRC_DIR%\\COPYING.LESSER" "%DEST_DIR%\\"

echo 'Running NSIS installer generator'
cd "%DEST_DIR%"
set PATH=%NSIS_PATH%;%PATH%
if "%OUT_FILE%" == "" (
  makensis /V4 %TULIP_NSI%
) else (
  makensis /V4 /DOUT_FILE=%OUT_FILE% %TULIP_NSI%
)

echo 'NSIS installer generator completed !'

rem final removal
rmdir /Q /S "%DEST_DIR%\\empty_folder" >nul 2>&1
