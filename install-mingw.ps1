# PowerShell MinGW Installation Script
# Requires 7-Zip to be installed

Write-Host "=========================================="
Write-Host "MinGW Installation Script"
Write-Host "=========================================="
Write-Host ""

# Step 1: Check for 7-Zip
Write-Host "[1/4] Checking for 7-Zip..."
$7zipPath = "C:\Program Files\7-Zip\7z.exe"
if (-not (Test-Path $7zipPath)) {
    Write-Host "[!] 7-Zip not found at $7zipPath"
    Write-Host ""
    Write-Host "Download and install 7-Zip from: https://www.7-zip.org/"
    Write-Host "Then run this script again."
    Write-Host ""
    Read-Host "Press Enter after installing 7-Zip"
}

# Step 2: Extract MinGW
Write-Host ""
Write-Host "[2/4] Extracting MinGW (this takes 1-2 minutes)..."
$mingwArchive = "$env:TEMP\mingw-installer.7z"
$mingwPath = "C:\MinGW"

if (Test-Path $mingwArchive) {
    & $7zipPath x $mingwArchive "-o$mingwPath" -aoa
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[OK] MinGW extracted successfully"
    } else {
        Write-Host "[ERROR] Extraction failed"
        exit 1
    }
} else {
    Write-Host "[ERROR] MinGW archive not found at $mingwArchive"
    exit 1
}

# Step 3: Verify files
Write-Host ""
Write-Host "[3/4] Verifying installation..."
$gccPath = "$mingwPath\bin\gcc.exe"
if (Test-Path $gccPath) {
    Write-Host "[OK] GCC found at $gccPath"
    & $gccPath --version | Select-Object -First 1
} else {
    Write-Host "[!] GCC not found - extraction may have failed"
}

# Step 4: Add to PATH
Write-Host ""
Write-Host "[4/4] Adding MinGW to system PATH..."
$mingwBin = "$mingwPath\bin"
$currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($currentPath -notlike "*$mingwBin*") {
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;$mingwBin", "User")
    Write-Host "[OK] MinGW added to PATH"
    Write-Host "[!] Please restart PowerShell for changes to take effect"
} else {
    Write-Host "[OK] MinGW already in PATH"
}

Write-Host ""
Write-Host "=========================================="
Write-Host "Setup Complete!"
Write-Host "=========================================="
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Restart PowerShell"
Write-Host "2. cd 'C:\Users\pc\Desktop\c graphics'"
Write-Host "3. gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32"
Write-Host "4. .\calculator.exe"
Write-Host ""
