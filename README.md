# Graphics Calculator - Setup & Installation Guide

A graphical calculator application built in C using the BGI (Basic Graphics Interface) graphics library for Windows, featuring a visual interface with interactive buttons.

## Table of Contents
- [Project Overview](#project-overview)
- [Downloaded Modules](#downloaded-modules)
- [System Requirements](#system-requirements)
- [Step-by-Step Setup](#step-by-step-setup)
- [Compilation & Execution](#compilation--execution)
- [Troubleshooting](#troubleshooting)

---

## Project Overview

This project implements a fully functional graphical calculator with:
- Visual button interface (numeric, operators, functions)
- Real-time display of calculations
- Support for basic arithmetic operations (+, -, *, /)
- Windows GUI integration using BGI graphics library
- Compiled C executable with no runtime dependencies

---

## Downloaded Modules

The following modules and libraries have been downloaded and included in this project:

### 1. **MinGW-w64 Compiler Suite** (`C:\MinGW\`)
   - **GCC Compiler**: `C:\MinGW\bin\gcc.exe`
   - **G++ Compiler**: `C:\MinGW\bin\g++.exe`
   - **Binutils**: Assembler, linker, and other tools
   - **Runtime Libraries**: Standard C/C++ libraries and Windows API support
   - **Purpose**: Compiles C source code to executable Windows applications

### 2. **BGI Graphics Library** (WinBGIm)
   - **Location**: Project root directory
   - **Components**:
     - `graphics.h` - Header file with graphics function declarations
     - `libbgi.c` - Implementation of BGI graphics functions for Windows
   - **Purpose**: Provides graphical drawing capabilities (windows, buttons, text, shapes)
   - **Windows APIs Used**: GDI (Graphics Device Interface), window management

### 3. **Project Source Files**
   - `calculator.c` - Main calculator application with logic and UI
   - Includes: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `math.h`, `windows.h`, `windowsx.h`

---

## System Requirements

- **OS**: Windows 7, 8, 10, 11 (32-bit or 64-bit)
- **Memory**: Minimum 512 MB RAM
- **Disk Space**: ~200 MB for MinGW installation
- **Software**:
  - PowerShell or Command Prompt
  - 7-Zip (for extracting .7z files) - **Optional** (pre-extracted in this project)
  - Administrator access for system PATH modifications

---

## Step-by-Step Setup

### **Option 1: Automated Setup (Recommended)**

If MinGW is not yet installed on your system, use the provided setup script:

#### Using Python:
```powershell
cd "C:\Users\pc\Desktop\c graphics"
python setup.py
```

#### Using PowerShell:
```powershell
cd "C:\Users\pc\Desktop\c graphics"
powershell -ExecutionPolicy Bypass -File setup.ps1
```

#### Using Batch:
```powershell
cd "C:\Users\pc\Desktop\c graphics"
.\setup.bat
```

### **Option 2: Manual Setup**

#### Step 1: Download MinGW-w64
Download from one of these sources:
- **Official**: https://www.mingw-w64.org/
- **GitHub**: https://github.com/niXman/mingw-builds-binaries/releases (Recommended)
- **Nuwen**: https://nuwen.net/mingw.html (Easiest, pre-configured)

**Recommended Downloads**:
- From Nuwen: `mingw-14.2.0-without-git.7z`
- From GitHub: `x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z`

#### Step 2: Create MinGW Installation Directory
```powershell
New-Item -ItemType Directory -Force -Path "C:\MinGW\include"
New-Item -ItemType Directory -Force -Path "C:\MinGW\lib"
New-Item -ItemType Directory -Force -Path "C:\MinGW\bin"
```

#### Step 3: Extract MinGW to C:\MinGW\
- Install **7-Zip** from https://www.7-zip.org/ (if not already installed)
- Right-click the downloaded `.7z` file
- Select **7-Zip → Extract to C:\MinGW\**
- Wait for extraction to complete

#### Step 4: Copy Graphics Library Files
**Option A** (If files are in this project):
```powershell
Copy-Item "graphics.h" -Destination "C:\MinGW\include\"
Copy-Item "libbgi.c" -Destination "C:\MinGW\lib\"
```

**Option B** (If you have the files elsewhere):
- Ensure `graphics.h` is in `C:\MinGW\include\`
- Ensure `libbgi.c` is in `C:\MinGW\lib\`

#### Step 5: Add MinGW to System PATH
1. Press **`Win + R`** → Type **`sysdm.cpl`** → Press **Enter**
2. Click **"Environment Variables"** button
3. Under **"System variables"**, find **`Path`** and click **"Edit"**
4. Click **"New"** and add: `C:\MinGW\bin`
5. Click **OK** on all dialogs
6. **Restart PowerShell** for changes to take effect

#### Step 6: Verify Installation
```powershell
gcc --version
```
You should see GCC version information (e.g., `gcc (GCC) 13.2.0`).

If "gcc is not recognized":
- Verify `C:\MinGW\bin\gcc.exe` exists
- Restart PowerShell
- Check System PATH settings

---

## Compilation & Execution

### **Compile the Calculator**

From the project directory:

```powershell
cd "C:\Users\pc\Desktop\c graphics"
gcc -I. calculator.c libbgi.c -o calculator.exe -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

**Explanation**:
- `-I.` - Include current directory for `graphics.h`
- `calculator.c` - Main calculator source
- `libbgi.c` - Graphics library implementation
- `-o calculator.exe` - Output executable name
- `-lgdi32`, `-lcomdlg32`, `-luuid`, `-loleaut32`, `-lole32` - Windows libraries for graphics and UI

### **Run the Calculator**

```powershell
.\calculator.exe
```

A graphics window will open with the calculator interface. Click buttons to perform calculations.

---

## Troubleshooting

### **"gcc is not recognized as an internal or external command"**
- **Cause**: MinGW not installed or not in system PATH
- **Solution**:
  1. Verify `C:\MinGW\bin\gcc.exe` exists
  2. Add `C:\MinGW\bin` to System PATH (Step 5 above)
  3. Restart PowerShell completely
  4. Run `gcc --version` to verify

### **"graphics.h: No such file or directory"**
- **Cause**: Header file not found
- **Solution**:
  1. Copy `graphics.h` to `C:\MinGW\include\`
  2. Verify the file exists: `Get-Item C:\MinGW\include\graphics.h`
  3. Recompile with `-I.` flag

### **"cannot find -lgdi32" or other library errors**
- **Cause**: Windows libraries not available or incorrect compiler
- **Solution**:
  1. Use x64 MinGW build (not 32-bit)
  2. Ensure MinGW is properly extracted to `C:\MinGW\`
  3. Reinstall MinGW if error persists

### **"libbgi.c: No such file or directory"**
- **Cause**: Graphics library implementation not in project directory
- **Solution**:
  1. Ensure `libbgi.c` is in `C:\Users\pc\Desktop\c graphics\`
  2. Verify with: `Get-Item libbgi.c`
  3. Re-extract or copy the file from the source

### **Compilation succeeds but .exe won't run**
- **Cause**: Missing Windows DLL dependencies
- **Solution**:
  1. Copy MinGW runtime DLLs to project directory:
     ```powershell
     Copy-Item "C:\MinGW\bin\libgcc_s_seh-1.dll" -Destination ".\"
     Copy-Item "C:\MinGW\bin\libwinpthread-1.dll" -Destination ".\"
     ```
  2. Try running again: `.\calculator.exe`

### **Window doesn't display properly**
- **Cause**: Graphics library initialization issue
- **Solution**:
  1. Ensure Windows has display drivers updated
  2. Try running on a different user account
  3. Rebuild from scratch:
     ```powershell
     Remove-Item calculator.exe
     gcc -I. calculator.c libbgi.c -o calculator.exe -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
     ```

---

## Project Files Explained

| File | Purpose |
|------|---------|
| `calculator.c` | Main source code with calculator logic and UI |
| `graphics.h` | BGI graphics library header (Windows compatible) |
| `libbgi.c` | BGI graphics library implementation for Windows |
| `calculator.exe` | Compiled executable (generated after compilation) |
| `setup.py` | Automated Python setup script |
| `setup.ps1` | Automated PowerShell setup script |
| `setup.bat` | Automated Batch setup script |
| `INSTALLATION_GUIDE.md` | Detailed installation reference |
| `README.md` | This file |

---

## Additional Notes

- The graphics library is a Windows-specific implementation of BGI (Basic Graphics Interface)
- The calculator uses GDI (Graphics Device Interface) for all drawing operations
- All compilation is done with GCC from MinGW-w64
- No external runtime dependencies beyond Windows OS libraries

---

## Support & References

- **MinGW-w64 Documentation**: https://www.mingw-w64.org/
- **BGI Graphics**: https://en.wikipedia.org/wiki/BGI
- **Windows GDI**: https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi
- **GCC Documentation**: https://gcc.gnu.org/

---

**Last Updated**: December 2025  
**Version**: 1.0
