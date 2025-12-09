# MinGW and BGI Installation Script for PowerShell
# Run as Administrator

Write-Host "=================================================="
Write-Host "MinGW and BGI Graphics Library Setup"
Write-Host "=================================================="
Write-Host ""

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "[!] Please run PowerShell as Administrator" -ForegroundColor Red
    exit
}

# Step 1: Check MinGW
Write-Host "Step 1: Checking for MinGW..." -ForegroundColor Cyan
if (Test-Path "C:\MinGW\bin\gcc.exe") {
    Write-Host "[OK] MinGW found at C:\MinGW" -ForegroundColor Green
} else {
    Write-Host "[!] MinGW not found" -ForegroundColor Yellow
    Write-Host "Download from: https://github.com/niXman/mingw-builds-binaries/releases" -ForegroundColor Yellow
    Write-Host "Extract to: C:\MinGW" -ForegroundColor Yellow
    Read-Host "Press Enter after extracting MinGW"
}

# Step 2: Create directories
Write-Host ""
Write-Host "Step 2: Creating MinGW directories..." -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path "C:\MinGW\include" | Out-Null
New-Item -ItemType Directory -Force -Path "C:\MinGW\lib" | Out-Null
Write-Host "[OK] Directories created" -ForegroundColor Green

# Step 3: Add to PATH
Write-Host ""
Write-Host "Step 3: Adding MinGW to system PATH..." -ForegroundColor Cyan
$currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($currentPath -notlike "*C:\MinGW\bin*") {
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;C:\MinGW\bin", "Machine")
    Write-Host "[OK] PATH updated" -ForegroundColor Green
    Write-Host "[*] Restart PowerShell to apply changes" -ForegroundColor Yellow
} else {
    Write-Host "[OK] MinGW already in PATH" -ForegroundColor Green
}

# Step 4: Test GCC
Write-Host ""
Write-Host "Step 4: Testing GCC..." -ForegroundColor Cyan
$gccTest = & gcc --version 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "[OK] GCC is working" -ForegroundColor Green
    Write-Host $gccTest[0]
} else {
    Write-Host "[!] GCC test failed" -ForegroundColor Yellow
    Write-Host "Restart PowerShell and try again" -ForegroundColor Yellow
}

# Step 5: BGI Library instructions
Write-Host ""
Write-Host "Step 5: BGI Library Installation Instructions" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Download WinBGIm graphics library"
Write-Host "   - Search online for 'WinBGIm graphics library'"
Write-Host "   - Or download from archived sources"
Write-Host ""
Write-Host "2. Extract the ZIP file"
Write-Host "   - Extract to a temporary folder"
Write-Host ""
Write-Host "3. Copy files to MinGW:"
Write-Host "   - Copy 'graphics.h' to: C:\MinGW\include\"
Write-Host "   - Copy 'libbgi.a' to: C:\MinGW\lib\"
Write-Host ""
Write-Host "4. Compile your calculator:"
Write-Host "   cd 'C:\Users\pc\Desktop\c graphics'"
Write-Host "   gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32"
Write-Host ""
Write-Host "5. Run it:"
Write-Host "   .\calculator.exe"
Write-Host ""

Write-Host "=================================================="
Write-Host "Setup Complete!" -ForegroundColor Green
Write-Host "=================================================="
