#!/usr/bin/env python3
"""
MinGW and BGI Installation Helper
Extracts 7z files and sets up the graphics calculator
"""

import os
import sys
import shutil
import subprocess
import urllib.request
from pathlib import Path

def download_file(url, destination):
    """Download a file from URL to destination"""
    print(f"Downloading {os.path.basename(destination)}...")
    try:
        urllib.request.urlretrieve(url, destination)
        print(f"[OK] Downloaded to {destination}")
        return True
    except Exception as e:
        print(f"[ERROR] Download failed: {e}")
        return False

def extract_7z(archive_path, extract_to):
    """Extract 7z archive using system command"""
    print(f"Extracting {os.path.basename(archive_path)}...")
    
    # Try system 7z command first
    try:
        result = subprocess.run([
            '7z', 'x', archive_path,
            f'-o{extract_to}', '-aoa'
        ], capture_output=False, text=True)
        if result.returncode == 0:
            print(f"[OK] Extracted to {extract_to}")
            return True
        else:
            print(f"[ERROR] 7z extraction failed")
    except FileNotFoundError:
        pass
    
    # Try py7zr as fallback with single-threaded mode
    try:
        import py7zr
        print("[INFO] Using Python extraction (single-threaded)...")
        with py7zr.SevenZipFile(archive_path, 'r') as archive:
            archive.extractall(path=extract_to, use_bsdtar=False)
        print(f"[OK] Extracted to {extract_to}")
        return True
    except ImportError:
        print("[ERROR] 7z not found. Install 7-Zip from https://www.7-zip.org/")
        print("[INFO] After installing 7-Zip, run this script again")
        return False
    except Exception as e:
        print(f"[ERROR] Extraction failed: {e}")
        return False

def setup_mingw():
    """Setup MinGW installation"""
    mingw_path = Path("C:/MinGW")
    
    # Create necessary directories
    (mingw_path / "include").mkdir(parents=True, exist_ok=True)
    (mingw_path / "lib").mkdir(parents=True, exist_ok=True)
    (mingw_path / "bin").mkdir(parents=True, exist_ok=True)
    
    print(f"[OK] MinGW directories created at {mingw_path}")
    
    # Create cache directory
    cache_dir = Path.home() / ".cache"
    cache_dir.mkdir(parents=True, exist_ok=True)
    
    # Download MinGW if not exists
    mingw_file = cache_dir / "mingw-installer.7z"
    if not mingw_file.exists():
        mingw_url = "https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev1/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z"
        print("[*] Downloading MinGW (this may take 1-2 minutes)...")
        if not download_file(mingw_url, str(mingw_file)):
            print("[!] Download failed. You can manually download from:")
            print("    https://github.com/niXman/mingw-builds-binaries/releases")
            print("    and extract to C:\\MinGW")
            return False
    else:
        print(f"[OK] MinGW archive found at {mingw_file}")
    
    # Extract MinGW
    if not extract_7z(str(mingw_file), str(mingw_path)):
        return False
    
    return True

def setup_bgi():
    """Setup BGI graphics library"""
    mingw_include = Path("C:/MinGW/include")
    mingw_lib = Path("C:/MinGW/lib")
    
    # Graphics header should already exist
    if (mingw_include / "graphics.h").exists():
        print("[OK] graphics.h already present")
    else:
        print("[WARNING] graphics.h not found")
    
    if (mingw_lib / "libbgi.c").exists():
        print("[OK] libbgi.c already present")
    else:
        print("[WARNING] libbgi.c not found")
    
    return True

def add_to_path():
    """Add MinGW to system PATH"""
    mingw_bin = "C:\\MinGW\\bin"
    current_path = os.environ.get("PATH", "")
    
    if mingw_bin not in current_path:
        new_path = current_path + ";" + mingw_bin
        os.environ["PATH"] = new_path
        print(f"[OK] MinGW added to PATH")
    else:
        print(f"[OK] MinGW already in PATH")
    
    return True

def main():
    """Main setup routine"""
    print("=" * 60)
    print("Graphics Calculator - MinGW and BGI Setup")
    print("=" * 60)
    print()
    
    print("[1/4] Setting up MinGW...")
    if not setup_mingw():
        print("[ERROR] MinGW setup failed!")
        return False
    
    print()
    print("[2/4] Setting up BGI graphics library...")
    if not setup_bgi():
        print("[WARNING] BGI setup incomplete")
    
    print()
    print("[3/4] Adding MinGW to PATH...")
    if not add_to_path():
        print("[WARNING] PATH setup incomplete")
    
    print()
    print("[4/4] Verifying GCC installation...")
    try:
        result = subprocess.run(["gcc", "--version"], capture_output=True, text=True)
        if result.returncode == 0:
            print(f"[OK] GCC is working:")
            print(f"  {result.stdout.split(chr(10))[0]}")
        else:
            print("[WARNING] GCC test failed")
            print("Please restart your terminal and try again")
    except FileNotFoundError:
        print("[WARNING] GCC not found in PATH")
        print("Please restart your terminal")
    
    print()
    print("=" * 60)
    print("Setup Complete!")
    print("=" * 60)
    print()
    print("Next steps:")
    print("1. Open PowerShell")
    print("2. Navigate to: C:\\Users\\pc\\Desktop\\c graphics")
    print("3. Compile: gcc calculator.c -o calculator.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32")
    print("4. Run: .\\calculator.exe")
    print()
    
    return True

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
