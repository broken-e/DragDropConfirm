; based on example2.nsi from NSIS examples

;--------------------------------
!include "x64.nsh"
!include "Library.nsh"
;!include "MUI2.nsh"

; The name of the installer
Name DragDropConfirm

; The file to write
OutFile "DragDropConfirm_installer.exe"

; The default installation directory
InstallDir "$PROGRAMFILES64\DragDropConfirm"	

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\DragDropConfirm" "Install_Dir"  

; Request application privileges for Windows Vista
RequestExecutionLevel admin
;--------------------------------

VIProductVersion "1.1.0.0"
VIAddVersionKey ProductName "DragDropConfirm"
;VIAddVersionKey Comments ""
VIAddVersionKey CompanyName "broken-e.com"
VIAddVersionKey LegalCopyright "Trey Miller"
VIAddVersionKey FileDescription "DragDropConfirm by Trey Miller"
VIAddVersionKey FileVersion "1.1.0.0"
VIAddVersionKey ProductVersion "1.1.0.0"
VIAddVersionKey InternalName "DragDropConfirm Setup"
;VIAddVersionKey LegalTrademarks ""
;VIAddVersionKey OriginalFilename ""



LicenseData "license.rtf"

; Pages
Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;  !insertmacro MUI_PAGE_COMPONENTS
;  !insertmacro MUI_PAGE_DIRECTORY
;  !insertmacro MUI_PAGE_INSTFILES
  
;  !insertmacro MUI_UNPAGE_CONFIRM
;  !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------

; main stuff. 
Section "Program Files"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "readme.txt"
  WriteUninstaller "uninstall.exe"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\DragDropConfirm "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DragDropConfirm" "DisplayName" "DragDropConfirm"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DragDropConfirm" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DragDropConfirm" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DragDropConfirm" "NoRepair" 1
  
  ; the dll
	!define LIBRARY_SHELL_EXTENSION
  	${If} ${RunningX64}	
		!define LIBRARY_X64
		!insertmacro InstallLib REGDLL       NOTSHARED REBOOT_PROTECTED      DragDropConfirm_64.dll $INSTDIR\DragDropConfirm_64.dll $INSTDIR
	${EndIf}  
	!insertmacro InstallLib REGDLL       NOTSHARED REBOOT_PROTECTED      DragDropConfirm_32.dll $INSTDIR\DragDropConfirm_32.dll $INSTDIR
	
  
SectionEnd

; Optional shortcuts section (can be disabled by the user)
Section "Start Menu Shortcuts"
  SetShellVarContext all
  
  CreateDirectory "$SMPROGRAMS\DragDropConfirm"
  CreateShortCut "$SMPROGRAMS\DragDropConfirm\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\DragDropConfirm\readme.lnk" "$INSTDIR\readme.txt" "" "$INSTDIR\readme.txt" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; the dll  
  	${If} ${RunningX64}
		!insertmacro UninstallLib REGDLL       NOTSHARED REBOOT_PROTECTED       $INSTDIR\DragDropConfirm_64.dll
	${EndIf}
	!insertmacro UninstallLib REGDLL       NOTSHARED REBOOT_PROTECTED       $INSTDIR\DragDropConfirm_32.dll
	
  
  ; Remove files and uninstaller
  Delete $INSTDIR\readme.txt
  Delete $INSTDIR\uninstall.exe
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DragDropConfirm"
  DeleteRegKey HKLM SOFTWARE\DragDropConfirm

  SetShellVarContext all
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\DragDropConfirm\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\DragDropConfirm"
  RMDir "$INSTDIR"
  
  MessageBox MB_YESNO|MB_ICONQUESTION "It's a good idea to reboot after removing DragDropConfirm. Reboot now?" IDNO +2
	Reboot

SectionEnd
