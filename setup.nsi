; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "TeacherRel"

; The file to write
OutFile "Teacher.exe"

; The default installation directory
InstallDir $PROGRAMFILES\TeacherRel

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\TeacherRel" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "TeacherRel (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r "E:\SYCOS_9.0\teacher\TeacherRel\*"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\TeacherRel "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TeacherRel" "DisplayName" "TeacherRel"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TeacherRel" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TeacherRel" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TeacherRel" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\TeacherRel"
  CreateShortcut "$SMPROGRAMS\TeacherRel\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\TeacherRel\TeacherRel.lnk" "$INSTDIR\Teacher.exe" "" "$INSTDIR\Teacher.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TeacherRel"
  DeleteRegKey HKLM SOFTWARE\TeacherRel

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*
  RMDir  /r "$INSTDIR\bearer"
  RMDir  /r "$INSTDIR\iconengines"
  RMDir  /r "$INSTDIR\imageformats"
  RMDir  /r "$INSTDIR\platforms"
  RMDir  /r "$INSTDIR\translations"
  RMDir  "$INSTDIR"
  ;Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\TeacherRel\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\TeacherRel"
  RMDir "$INSTDIR"

SectionEnd
