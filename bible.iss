[Setup]
AppName=Bible-CLI
AppVersion=1.0
DefaultDirName={autopf}\Bible-CLI
OutputBaseFilename=Bible_CLI Setup
ArchitecturesInstallIn64BitMode=x64compatible
ChangesEnvironment=yes
Compression=lzma
AppPublisher=miski54
SetupIconFile=icon.ico
WizardStyle=modern
DisableDirPage=no
WizardImageFile=blank.bmp
WizardSmallImageFile=blank.bmp
UninstallDisplayName=Bible-CLI 

[Files]
Source: "bible.exe"; DestDir: "{app}"
Source: "libstdc++-6.dll"; DestDir: "{app}"
Source: "libgcc_s_seh-1.dll"; DestDir: "{app}"
Source: "libwinpthread-1.dll"; DestDir: "{app}"
Source: "books.json"; DestDir: "{app}"
Source: "bible\*"; DestDir: "{app}\bible"; Flags: recursesubdirs
Source: "readme.md"; DestDir: "{app}"
Source: "bible-cli.cmd"; DestDir: "{app}"; Flags: ignoreversion
Source: "icon.bmp"; Flags: dontcopy

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Check: NeedsAddPath('{app}')

[Code]
var
  WelcomePage: TWizardPage;
  IconImage: TBitmapImage;

procedure InitializeWizard;
var
  WelcomeLabel: TNewStaticText;
begin
  WelcomePage := CreateCustomPage(
    wpWelcome,
    'Bible-CLI Setup',
    'Made by miski54 on GitHub'
  );

  ExtractTemporaryFile('icon.bmp');

  IconImage := TBitmapImage.Create(WelcomePage);
  IconImage.Parent := WelcomePage.Surface;

  IconImage.Bitmap.LoadFromFile(
    ExpandConstant('{tmp}\icon.bmp')
  );

  IconImage.AutoSize := False;
  IconImage.Width := 164;
  IconImage.Height := 164;
  IconImage.Stretch := True;
  IconImage.Center := True;

  IconImage.Left :=
    (WelcomePage.SurfaceWidth - IconImage.Width) div 2;

  IconImage.Top := 20;

  WelcomeLabel := TNewStaticText.Create(WelcomePage);
  WelcomeLabel.Parent := WelcomePage.Surface;

  WelcomeLabel.Caption :=
    'Welcome to Bible-CLI!' + #13#10 +
    'Click Next to continue';

  WelcomeLabel.Font.Size := 18;
  WelcomeLabel.Font.Style := [fsBold];
  WelcomeLabel.Alignment := taCenter;
  WelcomeLabel.AutoSize := False;

  WelcomeLabel.Left := 0;
  WelcomeLabel.Width := WelcomePage.SurfaceWidth;
  WelcomeLabel.Height := 120;
  WelcomeLabel.Top := 200;
end;

procedure CurPageChanged(CurPageID: Integer);
var
  TextHeight: Integer;
begin
  if CurPageID = wpFinished then
  begin
    WizardForm.StatusLabel.Visible := False;
    WizardForm.FinishedHeadingLabel.Visible := False;

    WizardForm.FinishedLabel.Caption :=
      'Bible-CLI has been installed!' + #13#10 +
      'Thank you for installing Bible-CLI.' + #13#10 +
      'Open your Terminal/PowerShell and enjoy!' + #13#10 +
      'Made by miski54 on GitHub.';

    WizardForm.FinishedLabel.Font.Size := 14;
    WizardForm.FinishedLabel.Font.Style := [fsBold];
    WizardForm.FinishedLabel.Alignment := taCenter;
    WizardForm.FinishedLabel.AutoSize := False;

    TextHeight := 140;

    WizardForm.FinishedLabel.SetBounds(
      0,
      (WizardForm.InnerPage.ClientHeight - TextHeight) div 2,
      WizardForm.InnerPage.ClientWidth,
      TextHeight
    );
  end;
end;

function NeedsAddPath(Param: string): Boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(
    HKLM,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path',
    OrigPath
  ) then
  begin
    Result := True;
    Exit;
  end;

  Result := Pos(
    ';' + ExpandConstant(Param) + ';',
    ';' + OrigPath + ';'
  ) = 0;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  Path: string;
begin
  if CurStep = ssPostInstall then
  begin
    Path := ExpandConstant('{app}');

    if NeedsAddPath(Path) then
      RegWriteExpandStringValue(
        HKLM,
        'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
        'Path',
        GetEnv('PATH') + ';' + Path
      );
  end;
end;
