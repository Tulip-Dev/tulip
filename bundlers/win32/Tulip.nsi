!define MUI_ICON "files\share\tulip\bitmaps\logo32x32.ico"
!define ZIP2EXE_NAME `Tulip`
!define ZIP2EXE_OUTFILE `tulip-4.0.0_setup.exe`
!define ZIP2EXE_COMPRESSOR_LZMA
!define ZIP2EXE_INSTALLDIR `$PROGRAMFILES\Tulip`
!include `${NSISDIR}\Contrib\zip2exe\Base.nsh`
!include `${NSISDIR}\Contrib\zip2exe\Modern.nsh`

!insertmacro MUI_PAGE_LICENSE "COPYING"

!insertmacro SECTION_BEGIN
 SetShellVarContext all

 File /r files\*.*
 CreateShortCut "$SMPROGRAMS\Tulip\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"
 CreateShortCut "$DESKTOP\Tulip.lnk" "$INSTDIR\bin\tulip.exe" "" "$INSTDIR\share\tulip\bitmaps\logo32x32.ico"
 CreateShortCut "$SMPROGRAMS\Tulip\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

 WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip" "DisplayName" "Tulip"
 WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip" "UninstallString" "$INSTDIR\Uninstall.exe"
 WriteUninstaller "$INSTDIR\Uninstall.exe"
!insertmacro SECTION_END

Section "Uninstall"
  SetShellVarContext all
  Delete "$DESKTOP\Tulip.lnk"
  RMDir /r "$SMPROGRAMS\Tulip"
  RMDir /r "$INSTDIR"
  Delete $INSTDIR\Uninstall.exe
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tulip"
SectionEnd
