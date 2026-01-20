# Windows Platform Setup

This directory contains the Windows platform implementation for SpotiFLAC.

## Prerequisites

1. **Visual Studio 2019 or later** with C++ development tools
2. **Flutter SDK** configured for Windows development
3. **Go 1.21+** for building the backend DLL
4. **CMake 3.14+** (usually comes with Visual Studio)

## Building the Go Backend DLL

Before you can run the Windows app, you need to build the Go backend as a DLL:

```bash
# Navigate to the go_backend directory
cd go_backend

# Build the DLL for Windows
go build -buildmode=c-shared -o gobackend.dll ./gobackend
```

This will create `gobackend.dll` in the `go_backend` directory.

## Running the Application

1. **Copy the DLL**: Copy `gobackend.dll` to the Windows build output directory:
   ```bash
   # After building the Go DLL
   cp go_backend/gobackend.dll build/windows/runner/Release/
   # or for Debug builds:
   cp go_backend/gobackend.dll build/windows/runner/Debug/
   ```

2. **Run with Flutter**:
   ```bash
   flutter run -d windows
   ```

## Building a Release

To create a release build:

```bash
# Build the Flutter app
flutter build windows --release

# Copy the DLL to the release directory
cp go_backend/gobackend.dll build/windows/runner/Release/

# The complete application will be in:
# build/windows/runner/Release/
```

## Architecture

### Plugin System

The Windows implementation uses a custom C++ plugin (`gobackend_plugin.cpp`) that:

1. **Loads gobackend.dll** dynamically at runtime
2. **Creates two MethodChannels**:
   - `com.zarz.spotiflac/backend` - for Go backend calls
   - `com.zarz.spotiflac/ffmpeg` - for FFmpeg operations (stub on Windows)
3. **Calls Go functions** via DLL exports
4. **Handles string conversions** between Dart/C++/Go

### Method Channel Implementation

The plugin handles all the same method calls as the Android version:

- Spotify/Deezer metadata fetching
- Track searching
- Download operations
- Extension system
- Lyrics fetching
- Cache management
- And more...

### FFmpeg Support

Currently, FFmpeg operations return a "not implemented" error on Windows. To add FFmpeg support:

1. Download FFmpeg for Windows
2. Implement FFmpeg command execution in `HandleFFmpegMethodCall`
3. Or use the extension system for audio conversion

## Troubleshooting

### DLL Not Found

If you get a "Failed to load gobackend.dll" error:

1. Make sure you've built the Go DLL
2. Copy it to the same directory as the .exe file
3. Check that the DLL is for the correct architecture (x64)

### Missing Functions

If you get "Function not found" errors:

1. Make sure your Go backend exports all required functions
2. Rebuild the DLL with: `go build -buildmode=c-shared`
3. Check that function names match exactly (case-sensitive)

### Build Errors

If CMake or Visual Studio fail to build:

1. Make sure you have the C++ development tools installed
2. Run `flutter doctor` to check for missing dependencies
3. Try cleaning: `flutter clean` then rebuild

## Icon

To use a custom icon:

1. Convert `icon.png` to `app_icon.ico` with multiple sizes
2. Place it in `runner/resources/app_icon.ico`
3. Rebuild the application

See `runner/resources/README.md` for icon conversion instructions.

## Distribution

When distributing your app, include:

1. The `.exe` file from `build/windows/runner/Release/`
2. `gobackend.dll` in the same directory
3. The `data` folder (contains Flutter assets)
4. Any other DLLs from the Release folder

You can use tools like Inno Setup or NSIS to create an installer.
