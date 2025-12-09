# Graphics Calculator - Complete Installation Guide

## Quick Start (Automated)

### Option A: Using Python Helper Script
1. Open PowerShell **as Administrator**
2. Navigate to: `C:\Users\pc\Desktop\c graphics`
3. Run: `python setup.py`
4. Follow the prompts

### Option B: Manual Installation

## Step-by-Step Manual Installation

### Step 1: Create MinGW Directories
```powershell
New-Item -ItemType Directory -Force -Path "C:\MinGW\include"
New-Item -ItemType Directory -Force -Path "C:\MinGW\lib"
New-Item -ItemType Directory -Force "C:\MinGW\bin"
```

### Step 2: Download and Extract MinGW-w64
1. **Download MinGW-w64** from one of these sources:
   - Official: https://www.mingw-w64.org/
   - GitHub Releases: https://github.com/niXman/mingw-builds-binaries/releases
   - Nuwen (Easiest): https://nuwen.net/mingw.html

2. **For Nuwen package (recommended):**
   - Download `mingw-14.2.0-without-git.7z` (or latest)
   - Extract to `C:\MinGW`

3. **For GitHub releases:**
   - Download `x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z`
   - Use 7-Zip to extract to `C:\MinGW`

**To extract .7z files:**
- Install 7-Zip from: https://www.7-zip.org/
- Right-click the .7z file → 7-Zip → Extract to `C:\MinGW`

### Step 3: Install BGI Graphics Library
The necessary files are already included:
- `graphics.h` is in `C:\MinGW\include\`
- `libbgi.c` is in `C:\MinGW\lib\`

These are specially configured for Windows compatibility.

### Step 4: Add MinGW to System PATH
1. Press `Win+R` and type `sysdm.cpl` → Enter
2. Click "Environment Variables" button
3. Under "System variables", find `Path` and click "Edit"
4. Click "New" and add: `C:\MinGW\bin`
5. Click OK on all dialogs
6. **Restart PowerShell** for changes to take effect

### Step 5: Verify Installation
```powershell
gcc --version
```
Should show GCC version information. If not, restart PowerShell.

## Compiling the Calculator

### In PowerShell:
```powershell
cd "C:\Users\pc\Desktop\c graphics"
gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

### Run the calculator:
```powershell
.\calculator.exe
```

## Troubleshooting

### "gcc is not recognized"
- MinGW not installed or not in PATH
- Restart PowerShell after adding to PATH
- Verify `C:\MinGW\bin\gcc.exe` exists

### "cannot find -lbgi"
- `libbgi.c` not copied to `C:\MinGW\lib\`
- Try simpler compile: `gcc calculator.c -o calculator.exe -lgdi32`

### "graphics.h: No such file or directory"
- `graphics.h` not copied to `C:\MinGW\include\`
- Verify file exists at `C:\MinGW\include\graphics.h`

### Compilation errors with graphics functions
- Ensure you're using the correct graphics.h provided in this project
- The header includes Windows API functions required for Windows

## Files Included

- `calculator.c` - Main calculator source code
- `graphics.h` (in `C:\MinGW\include\`) - BGI graphics header for Windows
- `libbgi.c` (in `C:\MinGW\lib\`) - BGI graphics library source
- `setup.py` - Automated Python setup script
- `setup.ps1` - PowerShell setup script
- `setup.bat` - Batch setup script
- `INSTALLATION_GUIDE.md` - This file

## Features

- **Visual calculator** with mouse-clickable buttons
- **Color-coded buttons** (blue for numbers, red for operators, green for equals)
- **Display screen** showing current input/results
- **Decimal support** for floating-point calculations
- **Error handling** (division by zero prevention)
- **Clear (C)** and Delete (Del) buttons for input correction

## Button Layout

```
7  8  9  /
4  5  6  *
1  2  3  -
0  .  =  +
C  Del
```

## Support

If you encounter issues:
1. Verify MinGW is installed at `C:\MinGW`
2. Check PATH includes `C:\MinGW\bin`
3. Confirm graphics.h is in `C:\MinGW\include\`
4. Try the Python setup script: `python setup.py`

Good luck with your graphics calculator!

### Step 4: Verify Installation
Open PowerShell and run:
```powershell
gcc --version
```
You should see the GCC version. If not, MinGW isn't in your PATH.

---

## Method 2: Using Package Manager (Easier)

### Using Chocolatey (if installed)
```powershell
choco install mingw
choco install bgi
```

### Using MSYS2
1. Install MSYS2 from: https://www.msys2.org/
2. Open MSYS2 terminal
3. Run:
```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-bgi
```

---

## Step 5: Update Your Code

Use this corrected header section in your C file:

```c
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
```

---

## Step 6: Compile Your Calculator

### In PowerShell, navigate to your project:
```powershell
cd "C:\Users\pc\Desktop\c graphics"
```

### Compile with this command:
```powershell
gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

### Run the calculator:
```powershell
.\calculator.exe
```

---

## Troubleshooting

### Error: "cannot find -lbgi"
- The library file wasn't copied to the right location
- Verify `libbgi.a` is in `C:\MinGW\lib\`
- Check your MinGW installation path

### Error: "graphics.h: No such file or directory"
- The header file wasn't copied
- Verify `graphics.h` is in `C:\MinGW\include\`

### Error: "gcc is not recognized"
- MinGW not in PATH
- Reinstall and ensure "Add to PATH" is selected
- Restart PowerShell after changing PATH

### Error: "undeclared identifier" for graphics functions
- Make sure you're including `<windows.h>` as well
- Verify you're using WinBGIm, not old BGI

---

## Quick Setup Summary

1. **Install MinGW** → Add to PATH
2. **Download WinBGIm** → Extract files
3. **Copy files** → `graphics.h` to include, `libbgi.a` to lib
4. **Compile** → Use the gcc command above
5. **Run** → Execute the `.exe` file

If you follow these steps carefully, your calculator should compile and run!
