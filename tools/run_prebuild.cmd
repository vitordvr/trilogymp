@echo off

rem Generate Project Solution
cd ..
utils\premake5.exe --file=source\premake5.lua vs2022

rem Create a shortcut to the solution - http://superuser.com/questions/392061/how-to-make-a-shortcut-from-cmd
set SCRIPTFILE="%TEMP%\CreateMyShortcut.vbs"
(
  echo Set oWS = WScript.CreateObject^("WScript.Shell"^)
  echo sLinkFile = oWS.ExpandEnvironmentStrings^("DMP.sln - Shortcut.lnk"^)
  echo Set oLink = oWS.CreateShortcut^(sLinkFile^)
  echo oLink.TargetPath = oWS.ExpandEnvironmentStrings^("%~dp0..\build\DMP.sln"^)
  echo oLink.Save
) 1>%SCRIPTFILE%
cscript //nologo %SCRIPTFILE%
del /f /q %SCRIPTFILE%

if %0 == "%~0" pause
