#!/bin/bash

# Build script for SpotiFLAC Mobile - Windows
# This script builds the Windows desktop version

set -e

echo "========================================"
echo "SpotiFLAC Mobile - Windows Build Script"
echo "========================================"
echo ""

# Check if Flutter is installed
if ! command -v flutter &> /dev/null; then
    echo "Error: Flutter is not installed or not in PATH"
    echo "Please install Flutter from https://flutter.dev/docs/get-started/install"
    exit 1
fi

# Check if Go is installed
if ! command -v go &> /dev/null; then
    echo "Error: Go is not installed or not in PATH"
    echo "Please install Go from https://golang.org/dl/"
    exit 1
fi

echo "Step 1: Building Go backend for Windows..."
cd go_backend

# Create output directory
mkdir -p ../windows/libs

# Build Go backend as shared library for Windows
echo "Building gobackend.dll..."
go build -buildmode=c-shared -o ../windows/libs/gobackend.dll .

if [ ! -f ../windows/libs/gobackend.dll ]; then
    echo "Error: Failed to build gobackend.dll"
    exit 1
fi

echo "✓ Go backend built successfully"
cd ..

echo ""
echo "Step 2: Getting Flutter dependencies..."
flutter pub get

echo ""
echo "Step 3: Building Windows application..."
flutter build windows --release

if [ -d "build/windows/x64/runner/Release" ]; then
    echo ""
    echo "✓ Build successful!"
    echo ""
    echo "Output location: build/windows/x64/runner/Release/"
    echo ""
    echo "To run the application:"
    echo "  cd build/windows/x64/runner/Release"
    echo "  ./spotiflac_android.exe"
    echo ""
    echo "To distribute, copy the entire Release folder including:"
    echo "  - spotiflac_android.exe"
    echo "  - All DLL files"
    echo "  - data/ folder"
    echo "  - gobackend.dll from windows/libs/"
else
    echo "Error: Build output not found"
    exit 1
fi
