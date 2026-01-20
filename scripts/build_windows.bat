@echo off
REM Build script for SpotiFLAC Mobile - Windows
REM This script builds the Windows desktop version

echo ========================================
echo SpotiFLAC Mobile - Windows Build Script
echo ========================================
echo.

REM Check if Flutter is installed
where flutter >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: Flutter is not installed or not in PATH
    echo Please install Flutter from https://flutter.dev/docs/get-started/install
    exit /b 1
)

REM Check if Go is installed
where go >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: Go is not installed or not in PATH
    echo Please install Go from https://golang.org/dl/
    exit /b 1
)

echo Step 1: Building Go backend for Windows...
cd go_backend

REM Create output directory
if not exist "..\windows\libs" mkdir "..\windows\libs"

REM Build Go backend as shared library for Windows
echo Building gobackend.dll...
go build -buildmode=c-shared -o ..\windows\libs\gobackend.dll .

if not exist "..\windows\libs\gobackend.dll" (
    echo Error: Failed to build gobackend.dll
    exit /b 1
)

echo √ Go backend built successfully
cd ..

echo.
echo Step 2: Getting Flutter dependencies...
call flutter pub get

echo.
echo Step 3: Building Windows application...
call flutter build windows --release

if exist "build\windows\x64\runner\Release" (
    echo.
    echo √ Build successful!
    echo.
    echo Output location: build\windows\x64\runner\Release\
    echo.
    echo To run the application:
    echo   cd build\windows\x64\runner\Release
    echo   spotiflac_android.exe
    echo.
    echo To distribute, copy the entire Release folder including:
    echo   - spotiflac_android.exe
    echo   - All DLL files
    echo   - data\ folder
    echo   - gobackend.dll from windows\libs\
) else (
    echo Error: Build output not found
    exit /b 1
)

pause
