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

Section "Tulip"
 SetShellVarContext all
 SetOutPath $INSTDIR

 File /r files\*.*
 CreateShortCut "$SMPROGRAMS\Tulip\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"
 CreateShortCut "$DESKTOP\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"
 CreateShortCut "$SMPROGRAMS\Tulip\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

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
