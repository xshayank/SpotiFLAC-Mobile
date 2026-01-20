# Building SpotiFLAC Mobile

This document describes how to build SpotiFLAC Mobile for different platforms.

## Prerequisites

- Flutter SDK (latest stable)
- Go 1.21 or later
- Platform-specific tools (see below)

## Platform-Specific Requirements

### Android
- Android SDK
- Android NDK r27d LTS (27.3.13750724)
- Java 17

### iOS
- macOS
- Xcode
- CocoaPods

### Windows
- Visual Studio 2022 or later with:
  - Desktop development with C++
  - Windows 10/11 SDK
- CMake (included with Visual Studio)

## Setup Instructions

### 1. Install Dependencies

```bash
# Get Flutter dependencies
flutter pub get

# Install gomobile (for Go backend)
go install golang.org/x/mobile/cmd/gomobile@latest
gomobile init
```

### 2. Enable Windows Platform (First Time Only)

```bash
# Enable Windows desktop support
flutter config --enable-windows-desktop

# Create Windows platform files
flutter create --platforms=windows .
```

### 3. Build Go Backend

#### For Android

```bash
cd go_backend

# Build for Android (creates AAR library)
mkdir -p ../android/app/libs
gomobile bind -target=android -androidapi 24 -o ../android/app/libs/gobackend.aar .

cd ..
```

#### For Windows

```bash
cd go_backend

# Build for Windows (creates DLL)
mkdir -p ../windows/libs
go build -buildmode=c-shared -o ../windows/libs/gobackend.dll .

cd ..
```

## Building

### Android APK

```bash
# Build release APK
flutter build apk --release

# Build split APKs per ABI
flutter build apk --release --split-per-abi
```

Output: `build/app/outputs/flutter-apk/`

### Windows EXE

```bash
# Build release EXE
flutter build windows --release
```

Output: `build/windows/x64/runner/Release/`

The Windows build will create:
- `spotiflac_android.exe` - Main executable
- Required DLL files
- `data/` folder with Flutter assets

To distribute:
1. Copy the entire `Release` folder
2. Include the `gobackend.dll` from `windows/libs/`
3. Package as ZIP or create an installer

## Proxy Support

SpotiFLAC now supports HTTP and SOCKS5 proxies.

### Configuring Proxy

1. Open the app
2. Go to **Settings**
3. Navigate to **Network** or **Advanced** settings
4. Enable **Use Proxy**
5. Configure:
   - **Type**: HTTP, HTTPS, or SOCKS5
   - **Host**: Proxy server address (e.g., `127.0.0.1`)
   - **Port**: Proxy port (e.g., `1080` for SOCKS5, `8080` for HTTP)
   - **Username** (optional): Proxy authentication username
   - **Password** (optional): Proxy authentication password

### Supported Proxy Types

- **HTTP**: Standard HTTP proxy
- **HTTPS**: HTTP proxy with TLS
- **SOCKS5**: SOCKS5 proxy with or without authentication

### Testing Proxy Configuration

After configuring the proxy:
1. Try searching for a track
2. Check the logs (Settings > Logs) for proxy-related messages
3. If connection fails, verify your proxy settings

## Troubleshooting

### Windows Build Issues

**Error: "gobackend.dll not found"**
- Make sure you built the Go backend for Windows
- Check that `windows/libs/gobackend.dll` exists
- Copy the DLL to the output folder if needed

**Error: "Visual Studio not found"**
- Install Visual Studio 2022 with Desktop development with C++
- Ensure Windows SDK is installed

### Android Build Issues

**Error: "gobackend.aar not found"**
- Build the Go backend for Android first
- Check that `android/app/libs/gobackend.aar` exists

**NDK version mismatch**
- Install NDK r27d LTS (27.3.13750724)
- Set `ANDROID_NDK_HOME` environment variable

### Proxy Issues

**Connection timeout with proxy**
- Verify proxy server is running
- Check proxy host and port are correct
- Test proxy with another application
- Try switching between HTTP and SOCKS5

**"ISP blocking detected"**
- Some ISPs block certain domains
- Try using the proxy to bypass ISP blocks
- Consider using a VPN in addition to proxy

## Development

### Running in Debug Mode

```bash
# Android
flutter run

# Windows
flutter run -d windows
```

### Hot Reload

Press `r` in the terminal to hot reload, or `R` to hot restart.

### Generating Code

When you modify files with `@JsonSerializable` or Riverpod annotations:

```bash
dart run build_runner build --delete-conflicting-outputs
```

## Release Builds

### Android

For signed release builds, you need:
1. Keystore file
2. Key properties configured in `android/key.properties`

### Windows

For production Windows builds:
1. Code signing certificate (optional but recommended)
2. Installer creation tool (e.g., Inno Setup, WiX, or NSIS)

## CI/CD

The project includes GitHub Actions workflows for automated builds:
- `.github/workflows/release.yml` - Builds and releases Android and iOS
- You can extend this for Windows builds

## Additional Resources

- [Flutter Documentation](https://docs.flutter.dev/)
- [Flutter Desktop Support](https://docs.flutter.dev/desktop)
- [Go Mobile Documentation](https://pkg.go.dev/golang.org/x/mobile)
