; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
;
; (c) Copyright 2004-2010, The TIEmu Team
;
; $Id$

[Setup]
AppName=SkinEdit
AppVerName=SkinEdit 1.28
AppPublisher=The TiLP Team
AppPublisherURL=http://lpg.ticalc.org/prj_tiemu/index.html
AppSupportURL=http://lpg.ticalc.org/prj_tiemu/index.html
AppUpdatesURL=http://lpg.ticalc.org/prj_tiemu/win32_download.html
DefaultDirName={pf}\SkinEdit
DefaultGroupName=SkinEdit
AllowNoIcons=yes
LicenseFile=C:\tiemu\skinedit\trunk\COPYING
InfoBeforeFile=C:\tiemu\skinedit\trunk\README
InfoAfterFile=C:\tiemu\skinedit\trunk\ChangeLog

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4; Flags: unchecked

[Files]
; Glade files
Source: "C:\sources\roms\skinedit\glade\*.glade"; DestDir: "{app}\glade"; Flags: ignoreversion;

; Pixmaps files
Source: "C:\sources\roms\skinedit\pixmaps\*.xpm"; DestDir: "{app}\pixmaps"; Flags: ignoreversion;
Source: "C:\sources\roms\skinedit\build\msvc\skinedit.ico"; DestDir: "{app}"; Flags: ignoreversion;

; i18n files
Source: "C:\sources\roms\skinedit\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "skinedit.mo"; Flags: ignoreversion;

; Misc files
Source: "C:\sources\roms\skinedit\README"; DestDir: "{app}"; DestName: "Readme.txt"; Flags: ignoreversion
Source: "C:\sources\roms\skinedit\AUTHORS"; DestDir: "{app}"; DestName: "Authors.txt"; Flags: ignoreversion
Source: "C:\sources\roms\skinedit\CHANGELOG"; DestDir: "{app}"; DestName: "ChangeLog.txt"; Flags: ignoreversion
Source: "C:\sources\roms\skinedit\COPYING"; DestDir: "{app}"; DestName: "License.txt"; Flags: ignoreversion

; Doc format files
Source: "C:\sources\roms\skinedit\docs\TiEmu_skin_format.txt"; DestDir: "{app}\docs"; DestName: "TiEmu Skin Format.txt"; Flags: ignoreversion
Source: "C:\sources\roms\skinedit\docs\VTi_skin_format.txt"; DestDir: "{app}\docs"; DestName: "VTi Skin Format.txt"; Flags: ignoreversion

; Executable files
Source: "C:\sources\roms\skinedit\build\msvc\skinedit.exe"; DestDir: "{app}"; DestName: "skinedit.exe"; Flags: ignoreversion

; GTK+ specific
Source: "C:\Gtk2Dev\bin\gtkthemeselector.exe"; DestDir: "{app}";

; Downloader
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\*.dll"; DestDir: "{app}\wget";
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\wget.exe"; DestDir: "{app}\wget";
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\d_and_i.bat"; DestDir: "{app}\wget";

[Dirs]
Name: "{app}\My Skins"; Flags: uninsneveruninstall;

[INI]
Filename: "{app}\skinedit.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://lpg.ticalc.org/prj_tiemu"

[Icons]
Name: "{group}\SkinEdit"; Filename: "{app}\skinedit.exe"; WorkingDir: "{app}\My Skins"
Name: "{group}\SkinEdit on the Web"; Filename: "{app}\skinedit.url"
Name: "{group}\Uninstall SkinEdit"; Filename: "{uninstallexe}"
;Name: "{group}\User's Manual"; Filename: "{app}\help\Manual_en.html"
Name: "{group}\GTK theme selector"; Filename: "{app}\gtkthemeselector.exe";
Name: "{group}\Install GTK+ from web"; Filename: "{app}\wget\d_and_i.bat";

Name: "{userdesktop}\SkinEdit"; Filename: "{app}\skinedit.exe"; WorkingDir: "{app}\My Skins"; MinVersion: 4,4; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\SkinEdit"; Filename: "{app}\skinedit.exe"; WorkingDir: "{app}\My Skins"; MinVersion: 4,4; Tasks: quicklaunchicon

[Run]
Filename: "{app}\skinedit.exe"; Description: "Launch SkinEdit"; StatusMsg: "Running SkinEdit..."; Flags: postinstall nowait unchecked
Filename: "{app}\wget\d_and_i.bat"; Description: "Download and install GTK+"; StatusMsg: "Running ..."; Flags: nowait postinstall unchecked hidewizard;

[Registry]
; Register skinedit in the shell
Root: HKCR; Subkey: ".skn"; ValueType: string; ValueName: ""; ValueData: "SkinEdit";
Root: HKCR; Subkey: "SkinEdit"; ValueType: string; ValueName: ""; ValueData: "VTi/TiEmu skin";
Root: HKCR; Subkey: "SkinEdit\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\skinedit.exe"
Root: HKCR; Subkey: "SkinEdit\shell\open";  ValueType: string; ValueData: "Open with &SkinEdit"
Root: HKCR; Subkey: "SkinEdit\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "{app}\skinedit.exe %1"

[UninstallDelete]
Type: files; Name: "{app}\skinedit.url"

;; Taken from "http://www.dropline.net/gtk/support.php" with some customizations

[Code]
var
  Exists: Boolean;
  GtkPath: String;
  WimpPath: String;
  GtkVersion: String;

function GetGtkInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Path', GtkPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Path', GtkPath);
  end;
   Result := Exists
end;

function GetGtkVersionInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Version', GtkVersion);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Version', GtkVersion);
  end;
   Result := Exists
end;

function GetGtkPath (S: String): String;
begin
    Result := GtkPath;
end;

function GetGtkVersion (S: String): String;
begin
    Result := GtkVersion;
end;

function IsTiglUsbVersion3Mini (): Boolean;
var
  Version: String;
begin
  GetVersionNumbersString('C:\WinNT\System\TiglUsb.dll', Version);
  if CompareStr(Version, '3.0.0.0') < 0 then begin
    Result := false;
  end;
end;

function DisplayWarning(I: Integer): Boolean;
var
  S: String;
begin
  if(I = 1) then begin
    S := 'The GTK+ libraries are not installed: ';
  end;
  if(I = 2) then begin
    S := 'The GTK+ libraries are installed but the version is old: ';
  end;
  MsgBox(S + 'you will need the GTK+ 2.12.x Runtime Environnement! But, the installer can download and install it for you; simply think to check the box at the last tab/page. Otherwise, you can still download it from the start menu (start menu > programs > tilp > install gtk+ from the web).', mbError, MB_OK);
end;

function InitializeSetup(): Boolean;
begin
  // Retrieve GTK path
  Result := GetGtkInstalled ();
  if not Result then begin
    DisplayWarning(1);
  end;

  // Retrieve GTK version
  if Result then begin
    Result := GetGtkVersionInstalled ();

    // and check
    if CompareStr(GtkVersion, '2.12.10') < 0 then begin
      DisplayWarning(2);
    end;
  end;

  // Check version of USB driver
  if IsTiglUsbVersion3Mini() then begin
    MsgBox('SilverLink driver v2.x has been removed of your system. Now, TiLP/TiEmu requires v3.x (check out the README for download location).', mbError, MB_OK);
  end;

  // Check for non-NT and WiMP theme
  WimpPath := GtkPath + '\lib\gtk-2.0\2.4.0\engines\libwimp.dll';
  if FileExists(WimpPath) and not UsingWinNT() then begin
        MsgBox('Tip: you are running a non-NT platform with the GTK+ WiMP theme engine installed. If you get a lot of warnings about fonts in console, run the Gtk+ Theme Selector as provided in the start menu group of TiLP/TiEmu', mbError, MB_OK);
  end;

  Result := true;
end;
