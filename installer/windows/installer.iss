; PolygonalPrimordials Installer Script
; Inno Setup (https://jrsoftware.org/isdl.php)

#define MyAppName "Polygonal Primordials"
#define MyAppVersion "2.0.0"
#define MyAppPublisher "Polygonal Primordials Team"
#define MyAppURL "https://primordials.game"
#define MyAppExeName "PolygonalPrimordials.exe"
#define MyAppCopyright "© 2027 Polygonal Primordials Team"

[Setup]
AppId={{8B1A2C3D-4E5F-6A7B-8C9D-0E1F2A3B4C5D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=..\..\LICENSE
InfoBeforeFile=..\..\README.md
OutputDir=..\..\dist
OutputBaseFilename=PolygonalPrimordials-Setup-{#MyAppVersion}
SetupIconFile=..\..\packaging\icon.ico
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
UninstallDisplayIcon={app}\{#MyAppExeName}
MinVersion=10.0
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName} Setup
VersionInfoCopyright={#MyAppCopyright}
CreateUninstRegKey=yes
UninstallDisplayName={#MyAppName}
DefaultGroupDescription=Evolutionary ecosystem simulation

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\..\build\Release\PolygonalPrimordials.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsilent
Source: "..\..\build\Release\PolygonalPrimordialsWallpaper.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsilent
Source: "..\..\Content\*"; DestDir: "{app}\Content"; Flags: recursesubdirs createallsubdirs
Source: "..\..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\CHANGELOG.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{#MyAppName} Live Wallpaper"; Filename: "{app}\PolygonalPrimordialsWallpaper.exe"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#MyAppName}}"; Flags: nowait postinstall skipifsilent
