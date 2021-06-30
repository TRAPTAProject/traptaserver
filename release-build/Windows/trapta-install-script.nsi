!define PRODUCT_NAME "TRAPTA"
!define PRODUCT_VERSION "1.26"
!define PRODUCT_WEB_SITE "www.trapta.eu"

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_ICON "trapta.ico"

; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\TRAPTA.exe"
!insertmacro MUI_PAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "French"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "archives\TRAPTA1.26-install.exe"
InstallDir "$PROGRAMFILES64\TRAPTA"
ShowInstDetails show

Section "MainSection" SEC01
  CreateDirectory "$SMPROGRAMS\TRAPTA"
  CreateShortCut "$SMPROGRAMS\TRAPTA.lnk" "$INSTDIR\TRAPTA.exe"
  CreateShortCut "$DESKTOP\TRAPTA.lnk" "$INSTDIR\TRAPTA.exe"
  SetOutPath "$INSTDIR"
  File /nonfatal /a /r "TRAPTA\"
  
SectionEnd

