@echo off
chcp 65001 >nul
title Oxide Cheat Injector

echo ============================================
echo   Oxide Survival Island - PC Injector
echo ============================================
echo.

set GAME_PACKAGE=com.rust.oxidesurvival
set CHEAT_LIB=liboxide_cheat.so
set CHEAT_DELAY=5

:: Ищем ADB
set ADB=
if exist "%ProgramFiles%\BlueStacks_nxt\HD-Adb.exe" set ADB="%ProgramFiles%\BlueStacks_nxt\HD-Adb.exe" & goto :found
if exist "%ProgramFiles%\BlueStacks\HD-Adb.exe" set ADB="%ProgramFiles%\BlueStacks\HD-Adb.exe" & goto :found
if exist "%ProgramFiles%\LDPlayer\LDPlayer9\adb.exe" set ADB="%ProgramFiles%\LDPlayer\LDPlayer9\adb.exe" & goto :found
if exist "%ProgramFiles%\Microvirt\MEmu\adb.exe" set ADB="%ProgramFiles%\Microvirt\MEmu\adb.exe" & goto :found
if exist "%ProgramFiles%\Nox\bin\nox_adb.exe" set ADB="%ProgramFiles%\Nox\bin\nox_adb.exe" & goto :found
where adb >nul 2>nul && set ADB=adb && goto :found

echo [!] ADB not found!
pause
exit /b 1

:found
echo [*] ADB: %ADB%

:: Подключение
echo [*] Connecting...
%ADB% connect 127.0.0.1:5555 >nul 2>nul
%ADB% connect 127.0.0.1:5565 >nul 2>nul
%ADB% connect 127.0.0.1:5575 >nul 2>nul
%ADB% connect 127.0.0.1:62001 >nul 2>nul

:: Root
echo [*] Root...
%ADB% root >nul 2>nul
timeout /t 2 >nul
%ADB% wait-for-device >nul 2>nul

:: Проверка игры
echo [*] Checking game...
%ADB% shell "pm list packages" | findstr "%GAME_PACKAGE%" >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] Game not installed in emulator!
    pause
    exit /b 1
)

:: Пушим
echo [*] Pushing library...
if not exist "%~dp0%CHEAT_LIB%" (
    echo [!] %CHEAT_LIB% not found!
    pause
    exit /b 1
)
%ADB% push "%~dp0%CHEAT_LIB%" /data/local/tmp/%CHEAT_LIB%
%ADB% shell "chmod 755 /data/local/tmp/%CHEAT_LIB%"

:: Инжект
echo [*] Injecting...
%ADB% shell "setprop wrap.%GAME_PACKAGE% 'LD_PRELOAD=/data/local/tmp/%CHEAT_LIB% CHEAT_DELAY=%CHEAT_DELAY%'"
%ADB% shell "am force-stop %GAME_PACKAGE%"
timeout /t 1 >nul
%ADB% shell "monkey -p %GAME_PACKAGE% -c android.intent.category.LAUNCHER 1" >nul 2>nul

echo.
echo ============================================
echo   Done! Wait %CHEAT_DELAY% sec for menu.
echo   Run cleanup.bat to remove.
echo ============================================
pause
