!include "MUI.nsh"

; Global installer settings
Name "Tulip"
!define MUI_ICON "files\share\tulip\bitmaps\logo32x32.ico"
!define MUI_UNICON "files\share\tulip\bitmaps\logo32x32.ico"
RequestExecutionLevel admin

!define MUI_ABORTWARNING

; Installer pages
!insertmacro MUI_PAGE_LICENSE "COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

; Uninstaller pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Language
!insertmacro MUI_LANGUAGE "English"

InstallDir $PROGRAMFILES\Tulip
OutFile "tulip_setup.exe"

Function CheckAndInstallPython 
  ReadRegStr $0 HKLM SOFTWARE\Python\PythonCore\2.7\InstallPath ""
  IfErrors 0 +2
        ReadRegStr $0 HKLM SOFTWARE\Wow6432Node\Python\PythonCore\2.7\InstallPath ""
  IfErrors install end
  install:
        MessageBox MB_OK "Your system does not appear to have Python 2.7 installed.$\n$\nIt is now required to run Tulip.$\n$\nPress OK to download Python 2.7 and install it."
        NSISdl::download http://www.python.org/ftp/python/2.7.4/python-2.7.4.msi python-2.7.4.msi
        ExecWait '"msiexec" /i "python-2.7.4.msi"'
        Delete python-2.7.4.msi
  end:  
FunctionEnd

Section "Tulip"
 SetShellVarContext all
 SetOutPath $INSTDIR
 Call CheckAndInstallPython
 File /r /x *.dll.a /x Qt*d4.dll /x phonond4.dll files\*.*
 CreateDirectory "$SMPROGRAMS\Tulip"
 CreateShortCut "$SMPROGRAMS\Tulip\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"
 CreateShortCut "$SMPROGRAMS\Tulip\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
 CreateShortCut "$DESKTOP\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"

 WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip" "DisplayName" "Tulip"
 WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip" "UninstallString" "$INSTDIR\Uninstall.exe"
 WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
  SetShellVarContext all
  Delete "$DESKTOP\Tulip.lnk"
  RMDir /r "$SMPROGRAMS\Tulip"
  RMDir /r "$INSTDIR"
  Delete $INSTDIR\Uninstall.exe
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip"
SectionEnd
