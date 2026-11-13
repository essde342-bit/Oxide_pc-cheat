@echo off
echo Searching for ADB...
echo.

set FOUND=0

if exist "%ProgramFiles%\BlueStacks_nxt\HD-Adb.exe" (
    echo [OK] BlueStacks 5: %ProgramFiles%\BlueStacks_nxt\HD-Adb.exe
    set FOUND=1
)
if exist "%ProgramFiles%\BlueStacks\HD-Adb.exe" (
    echo [OK] BlueStacks 4: %ProgramFiles%\BlueStacks\HD-Adb.exe
    set FOUND=1
)
if exist "%ProgramFiles%\LDPlayer\LDPlayer9\adb.exe" (
    echo [OK] LDPlayer 9: %ProgramFiles%\LDPlayer\LDPlayer9\adb.exe
    set FOUND=1
)
if exist "%ProgramFiles%\Microvirt\MEmu\adb.exe" (
    echo [OK] MEmu: %ProgramFiles%\Microvirt\MEmu\adb.exe
    set FOUND=1
)
if exist "%ProgramFiles%\Nox\bin\nox_adb.exe" (
    echo [OK] Nox: %ProgramFiles%\Nox\bin\nox_adb.exe
    set FOUND=1
)

where adb >nul 2>nul
if %errorlevel%==0 (
    echo [OK] System ADB: adb
    set FOUND=1
)

if %FOUND%==0 echo [!] Nothing found.

echo.
pause
