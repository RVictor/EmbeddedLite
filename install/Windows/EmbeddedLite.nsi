; EmbeddedLite.nsi
;
; It will install EmbeddedLite.nsi into a directory that the user selects,

;--------------------------------

Function .onInit
  SetOutPath $TEMP
  File /oname=Splash.bmp "Splash.bmp"

  advsplash::show 2000 600 400 -1 $TEMP\Splash

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\Splash.bmp
FunctionEnd

LicenseData "license.txt"

; The name of the installer
Name "EmbeddedLite Setup"

; The file to write
OutFile "EmbeddedLiteSetup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\EmbeddedLite

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\EmbeddedLite" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages
Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "EmbeddedLite (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "..\..\build\WinReleaseUnicode\EmbeddedLite.exe"
  File "..\..\build\WinReleaseUnicode\*.dll"
  ; Copy the misc files
  File "..\..\build\WinReleaseUnicode\emblite_indexer.exe"
;  File "..\..\build\WinReleaseUnicode\emblite_cppcheck.exe"
;  File "..\..\build\WinReleaseUnicode\emblite_launcher.exe"
;  File "..\..\build\WinReleaseUnicode\makedir.exe"
;  File "..\..\build\WinReleaseUnicode\le_exec.exe"
;  File "..\..\build\WinReleaseUnicode\patch.exe"
  File "..\distr\astyle.sample"
;  File "..\..\build\WinReleaseUnicode\*.template"
  File "..\distr\*.html"
  File "license.txt"

  SetOutPath $INSTDIR\config
  File "..\distr\config\*.default"
  File  /oname=build_settings.xml.default "..\distr\config\build_settings.xml.default.win"

  SetOutPath $INSTDIR\debuggers
  File "..\..\build\WinReleaseUnicode\debuggers\*.dll"

  SetOutPath $INSTDIR\images
  File "..\distr\images\*.*"

  SetOutPath $INSTDIR\lexers
  File /r "..\distr\lexers\*.xml"

  SetOutPath $INSTDIR\plugins
  File /r "..\..\build\WinReleaseUnicode\plugins\*.dll"

  SetOutPath $INSTDIR\plugins\resources
  File "..\distr\plugins\resources\*.*"

  SetOutPath $INSTDIR\rc
  File /r "..\distr\rc\*.xrc"

  SetOutPath $INSTDIR\templates
  File /r "..\distr\templates\*.*"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\EmbeddedLite "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmbeddedLite" "DisplayName" "EmbeddedLite"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmbeddedLite" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmbeddedLite" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmbeddedLite" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\EmbeddedLite"
  CreateShortCut "$SMPROGRAMS\EmbeddedLite\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\EmbeddedLite\EmbeddedLite (MakeNSISW).lnk" "$INSTDIR\EmbeddedLite.nsi" "" "$INSTDIR\EmbeddedLite.nsi" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EmbeddedLite"
  DeleteRegKey HKLM SOFTWARE\EmbeddedLite

  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  Delete $INSTDIR\*.dll
  Delete $INSTDIR\astyle.sample
  Delete $INSTDIR\*.template
  Delete $INSTDIR\*.html
  Delete $INSTDIR\config\*.default
  Delete $INSTDIR\config\build_settings.xml.default.win
  Delete $INSTDIR\debuggers\*.dll
  Delete $INSTDIR\images\*.*
  Delete $INSTDIR\lexers\*.xml
  Delete $INSTDIR\plugins\*.dll
  Delete $INSTDIR\plugins\resources\*.*
  Delete $INSTDIR\rc\*.xrc
  Delete $INSTDIR\templates\*.*

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\EmbeddedLite\*.*"

  ; Remove directories used
;  RMDir "$SMPROGRAMS\EmbeddedLite"
  RMDir "$INSTDIR\debuggers"
  RMDir "$INSTDIR\images"
  RMDir "$INSTDIR\lexers"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\plugins\resources"
  RMDir "$INSTDIR\rc"
  RMDir "$INSTDIR\templates"
  RMDir "$INSTDIR"

SectionEnd
