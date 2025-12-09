@echo off
REM MinGW and BGI Installation Helper Script
REM Run this script in PowerShell as Administrator

echo.
echo ==================================================
echo MinGW and BGI Graphics Library Setup
echo ==================================================
echo.

REM Step 1: Check if MinGW exists
echo Step 1: Checking for MinGW...
if exist "C:\MinGW\bin\gcc.exe" (
    echo [OK] MinGW found at C:\MinGW
) else (
    echo [!] MinGW not found. Please download and extract to C:\MinGW
    echo Download from: https://github.com/niXman/mingw-builds-binaries/releases
    pause
)

REM Step 2: Add to PATH
echo.
echo Step 2: Adding MinGW to PATH...
setx PATH "%PATH%;C:\MinGW\bin"
echo [OK] PATH updated. Restart PowerShell for changes to take effect.

REM Step 3: Test GCC
echo.
echo Step 3: Testing GCC...
gcc --version
if errorlevel 1 (
    echo [!] GCC test failed. Restart PowerShell and try again.
) else (
    echo [OK] GCC is working
)

REM Step 4: Create required directories
echo.
echo Step 4: Creating MinGW directories...
if not exist "C:\MinGW\include" mkdir "C:\MinGW\include"
if not exist "C:\MinGW\lib" mkdir "C:\MinGW\lib"
echo [OK] Directories created

echo.
echo ==================================================
echo Setup Complete!
echo Next: Extract WinBGIm and copy graphics.h and libbgi.a
echo ==================================================
pause
