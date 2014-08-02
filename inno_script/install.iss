#define MyAppName "GOSTunnel GUI"
#define PF_Path "gostunnel-gui"
#define MyAppVersion "1.0"
#define MyAppPublisher "Dostovalov Vitaly"
#define MyAppURL "http://w3tl.github.io/gostunnel/"
#define MyAppExeName "gostunnel-gui.exe"

[Setup]
AppId={{299330FD-DC17-481D-8D48-DFEBCF4DF2F4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#PF_Path}
DefaultGroupName={#MyAppName}
LicenseFile=license_ru.txt
OutputDir=.
OutputBaseFilename=gostunnel-setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;

[Files]
Source: "../bin/gostunnel.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "gostunnel.conf"; DestDir: "{app}"; DestName: "gostunnel.conf"; Flags: ignoreversion; Permissions: users-readexec 
Source: "../bin/certs/*"; DestDir: "{app}/certs"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "license.txt"; DestDir: {app}; Flags: ignoreversion
Source: "license_ru.txt"; DestDir: {app}; Flags: ignoreversion; 
Source: "gostunnel.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "gostunnel-gui.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "create_cert.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "hosts.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "errors.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "install_local_crt.pdf"; DestDir: {app}/docs; Flags: ignoreversion;
Source: "C:\dev\builds\gostunnel-gui\release\gostunnel-gui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\config_deploy.txt"; DestDir: "{app}"; DestName: "config.txt"; Flags: ignoreversion;
Source: "C:\dev\builds\gostunnel-gui\release\platforms\qwindows.dll"; DestDir: "{app}/platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\dev\builds\gostunnel-gui\release\icudt51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\icuin51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\icuuc51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\msvcp100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\dev\builds\gostunnel-gui\release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion

[Dirs]
Name: "{app}/logs"; Permissions: users-modify users-readexec;
Name: "{app}/docs"; Permissions: users-readexec;

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\Руководство {#MyAppName}"; Filename: "{app}\docs\gostunnel-gui.pdf";
Name: "{group}\Руководство GOSTunnel"; Filename: "{app}\docs\gostunnel-gui.pdf";
Name: "{group}\Документация"; Filename: "{app}\docs";
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
