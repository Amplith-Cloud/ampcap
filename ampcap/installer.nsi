;Include Modern UI


!define MUI_COMPONENTSPAGE_NODESC

  !include "MUI.nsh"
  !include "x64.nsh"

;--------------------------------
;General

!searchparse /file ampcap_version.h '#define AMPCAP_VERSION "v' VER_MAJOR '.' VER_MINOR '"'

SetCompressor lzma



  ;Name and file
  Name "AmpCap v${VER_MAJOR}.${VER_MINOR}"
  OutFile "ampcap${VER_MAJOR}${VER_MINOR}-install.exe"

  ;Default installation folder (64-bit build)
  InstallDir "$PROGRAMFILES64\AmpCap"

  ;The previous installation folder is read from the 64-bit registry view
  ;in .onInit, since InstallDirRegKey only reads the 32-bit view

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"




;--------------------------------
;64-bit only

Function .onInit
  ${IfNot} ${RunningX64}
    MessageBox MB_OK|MB_ICONSTOP "AmpCap requires 64-bit Windows."
    Abort
  ${EndIf}
  SetRegView 64
  ReadRegStr $0 HKLM "Software\AmpCap" ""
  ${If} $0 != ""
    StrCpy $INSTDIR $0
  ${EndIf}
FunctionEnd

Function un.onInit
  SetRegView 64
FunctionEnd

;--------------------------------
;Installer Sections


Section "Required files"

  SectionIn RO
  SetOutPath "$INSTDIR"


  File release\AmpCap.exe
;  File release\AmpCap_cli.exe
    
  ;Store installation folder
  WriteRegStr HKLM "Software\AmpCap" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  File license.txt
  File whatsnew.txt

SectionEnd

Section "Desktop Icon"
  CreateShortcut "$DESKTOP\AmpCap.lnk" "$INSTDIR\AmpCap.exe"
SectionEnd

Section "Start Menu Shortcuts"

  SetOutPath $SMPROGRAMS\AmpCap
  CreateShortcut "$OUTDIR\AmpCap.lnk" "$INSTDIR\AmpCap.exe"
  CreateShortcut "$OUTDIR\AmpCap License.lnk" "$INSTDIR\license.txt"
  CreateShortcut "$OUTDIR\Whatsnew.txt.lnk" "$INSTDIR\whatsnew.txt"
  CreateShortcut "$OUTDIR\Uninstall AmpCap.lnk" "$INSTDIR\uninstall.exe"

  SetOutPath $INSTDIR

SectionEnd

;Section "AmpCap Source Code"
;  SetOutPath $INSTDIR\Source
;  File requires_wdl.txt
;
;  SetOutPath $INSTDIR\Source\ampcap
;  File installer.nsi
;  File license.txt
;  File ampcap.dsw
;  File ampcap_cli.dsp
;  File ampcap_gui.dsp
;  File icon1.ico
;  File ampcap.rc
;  File resource.h
;  File ampcap_version.h
;  File ampcap_cli.cpp
;  File ampcap_ui.cpp
;  File requires_wdl.txt
;  File whatsnew.txt
;  
;  File ampcap.icns
;  File capturewindow.mm
;
;  File ampcap-Info.plist
;  File main.m
;  File ampcap_Prefix.pch
;  File makedmg.sh
;  File pkg-dmg
;  File stage_DS_Store
;  File background.png
;
;  File /r English.lproj
;
;  SetOutPath $INSTDIR\Source\ampcap\ampcap.xcodeproj
;  File ampcap.xcodeproj\project.pbxproj
;
;SectionEnd
;


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  DeleteRegKey HKLM "Software\AmpCap"

  Delete "$INSTDIR\AmpCap_cli.exe"
  Delete "$INSTDIR\AmpCap.exe"

  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\whatsnew.txt"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$SMPROGRAMS\AmpCap\*.lnk"
  RMDir $SMPROGRAMS\AmpCap
  Delete "$DESKTOP\AmpCap.lnk"

  Delete $INSTDIR\Source\AmpCap\installer.nsi
  Delete $INSTDIR\Source\AmpCap\license.txt
  Delete $INSTDIR\Source\AmpCap\whatsnew.txt
  Delete $INSTDIR\Source\AmpCap\ampcap.dsw
  Delete $INSTDIR\Source\AmpCap\ampcap_cli.dsp
  Delete $INSTDIR\Source\AmpCap\ampcap_gui.dsp
  Delete $INSTDIR\Source\AmpCap\icon1.ico
  Delete $INSTDIR\Source\AmpCap\ampcap.rc
  Delete $INSTDIR\Source\AmpCap\resource.h
  Delete $INSTDIR\Source\AmpCap\ampcap_version.h
  Delete $INSTDIR\Source\AmpCap\ampcap_cli.cpp
  Delete $INSTDIR\Source\AmpCap\background.png
  Delete $INSTDIR\Source\AmpCap\ampcap_ui.cpp
  Delete $INSTDIR\Source\AmpCap\requires_wdl.txt
  Delete $INSTDIR\Source\requires_wdl.txt

  Delete $INSTDIR\Source\ampcap.icns
  Delete $INSTDIR\Source\capturewindow.mm

  Delete $INSTDIR\Source\makedmg.sh
  Delete $INSTDIR\Source\pkg-dmg
  Delete $INSTDIR\Source\stage_DS_Store

  Delete $INSTDIR\Source\ampcap-Info.plist
  Delete $INSTDIR\Source\main.m
  Delete $INSTDIR\Source\ampcap_Prefix.pch

  Delete "$INSTDIR\English.lproj\*.*"
  Delete "$INSTDIR\Source\ampcap\ampcap.xcodeproj\*.*"

  RMDir $INSTDIR\English.lproj
  RMDir  "$INSTDIR\Source\ampcap\ampcap.xcodeproj"

  RMDir $INSTDIR\Source\AmpCap
  RMDir $INSTDIR\Source
  
  RMDir "$INSTDIR"

SectionEnd
