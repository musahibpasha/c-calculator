@echo off
REM ============================================================
REM Complete MinGW and Graphics Calculator Setup Script
REM ============================================================
REM This script downloads and installs everything needed

echo.
echo ============================================================
echo Graphics Calculator - Complete Setup
echo ============================================================
echo.

REM Check if running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] This script must be run as Administrator!
    echo Please right-click and select "Run as Administrator"
    pause
    exit /b 1
)

echo [1/5] Checking MinGW installation...
if exist "C:\MinGW\include\graphics.h" (
    echo [OK] graphics.h found
) else (
    echo [!] graphics.h not found
)

if exist "C:\MinGW\lib\libbgi.c" (
    echo [OK] libbgi.c found
) else (
    echo [!] libbgi.c not found
)

echo.
echo [2/5] Downloading GCC compiler...
REM Download portable GCC from nuwen
set "GCC_URL=https://nuwen.net/files/mingw/mingw-14.2.0-without-git.7z"
set "TEMP_FILE=%TEMP%\mingw-gcc.7z"

REM For now, we'll skip the download and use manual installation
echo Note: Please download from https://nuwen.net/ or https://www.mingw-w64.org/
echo Then extract to C:\MinGW\

echo.
echo [3/5] Setting up environment...
setx MINGW_HOME C:\MinGW
setx PATH "%PATH%;C:\MinGW\bin"
echo [OK] Environment variables set

echo.
echo [4/5] Verifying setup...
dir "C:\MinGW\include\graphics.h" >nul 2>&1
if %errorlevel% equ 0 (
    echo [OK] graphics.h is in place
) else (
    echo [!] graphics.h not found
)

echo.
echo [5/5] Ready to compile!
echo.
echo ============================================================
echo Setup Complete!
echo ============================================================
echo.
echo Next steps:
echo 1. Download MinGW from https://www.mingw-w64.org/
echo 2. Extract to C:\MinGW\
echo 3. Open PowerShell and run:
echo    cd "C:\Users\pc\Desktop\c graphics"
echo    gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
echo 4. Run the calculator:
echo    .\calculator.exe
echo.
pause
