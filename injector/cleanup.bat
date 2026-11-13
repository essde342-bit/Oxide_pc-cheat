@echo off
chcp 65001 >nul
title Oxide Cheat Cleanup

set GAME_PACKAGE=com.rust.oxidesurvival

set ADB=
if exist "%ProgramFiles%\BlueStacks_nxt\HD-Adb.exe" set ADB="%ProgramFiles%\BlueStacks_nxt\HD-Adb.exe" & goto :found
if exist "%ProgramFiles%\BlueStacks\HD-Adb.exe" set ADB="%ProgramFiles%\BlueStacks\HD-Adb.exe" & goto :found
where adb >nul 2>nul && set ADB=adb && goto :found
echo [!] ADB not found & pause & exit /b 1

:found
%ADB% connect 127.0.0.1:5555 >nul 2>nul
%ADB% connect 127.0.0.1:5565 >nul 2>nul
timeout /t 1 >nul

echo [*] Removing...
%ADB% shell "setprop wrap.%GAME_PACKAGE% ''"
%ADB% shell "rm -f /data/local/tmp/liboxide_cheat.so"
%ADB% shell "am force-stop %GAME_PACKAGE%"

echo [*] Done.
pause
