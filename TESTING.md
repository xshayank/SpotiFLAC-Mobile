# Testing Guide for SpotiFLAC Mobile

This document provides guidance for testing the new proxy support and Windows platform features.

## Proxy Support Testing

### Prerequisites
- A proxy server (local or remote)
- SpotiFLAC Mobile installed on Android or Windows

### Recommended Test Proxies

For testing purposes, you can use:
1. **Local SOCKS5 proxy**: SSH tunnel with `ssh -D 1080 user@server`
2. **Local HTTP proxy**: Squid, Privoxy, or mitmproxy
3. **Public test proxies**: Search for free HTTP/SOCKS5 proxies (use with caution)

### Test Cases

#### 1. Basic Proxy Configuration

**Test HTTP Proxy:**
1. Open SpotiFLAC Mobile
2. Go to Settings > Proxy
3. Enable "Use Proxy"
4. Set proxy type to "HTTP"
5. Enter proxy host (e.g., `127.0.0.1` or `proxy.example.com`)
6. Enter proxy port (e.g., `8080`)
7. Save settings
8. Try searching for a track
9. Check logs (Settings > Logs) for proxy-related messages

**Expected Result:**
- All API requests should route through the proxy
- Search results should appear normally
- No connection errors

**Test SOCKS5 Proxy:**
1. Change proxy type to "SOCKS5"
2. Update port to SOCKS5 port (e.g., `1080`)
3. Save settings
4. Try searching for a track

**Expected Result:**
- Same as HTTP proxy test
- SOCKS5 connections should work

#### 2. Proxy Authentication

**Test with Username/Password:**
1. Configure proxy settings
2. Enter username in "Username" field
3. Enter password in "Password" field
4. Save settings
5. Try searching for a track

**Expected Result:**
- Authenticated requests should succeed
- No authentication errors in logs

#### 3. Proxy Error Handling

**Test Invalid Proxy:**
1. Set proxy host to invalid address (e.g., `invalid.proxy.local`)
2. Enable proxy
3. Save settings
4. Try searching for a track

**Expected Result:**
- Connection should fail with appropriate error message
- App should not crash
- Error message should mention proxy/connection issue

**Test Wrong Port:**
1. Set proxy to valid host but wrong port (e.g., `1234`)
2. Try searching

**Expected Result:**
- Connection timeout or refused error
- Helpful error message

#### 4. Proxy Toggle

**Test Enable/Disable:**
1. Configure working proxy
2. Disable "Use Proxy"
3. Try searching (should use direct connection)
4. Enable "Use Proxy"
5. Try searching (should use proxy)

**Expected Result:**
- Seamless switching between proxy and direct connection
- No residual proxy settings when disabled

#### 5. ISP Blocking Bypass

**Test with Blocked Service:**
1. If your ISP blocks Tidal/Qobuz/Amazon Music
2. Configure proxy to route through different location
3. Try downloading a track

**Expected Result:**
- Download should succeed through proxy
- ISP block should be bypassed

### Proxy Verification

#### Check if Proxy is Being Used:

**Method 1: Proxy Server Logs**
- Check your proxy server logs for incoming connections
- You should see requests from SpotiFLAC

**Method 2: Network Monitor**
- Use Wireshark or tcpdump
- Filter for connections to proxy port
- Verify traffic goes through proxy, not direct to API servers

**Method 3: App Logs**
- Settings > Logs
- Look for messages like: `[Proxy] Configured http proxy: 127.0.0.1:8080`
- Look for connection success/failure messages

## Windows Build Testing

### Prerequisites
- Windows 10/11
- Flutter SDK installed
- Go 1.21+ installed
- Visual Studio 2022 with C++ development tools

### Building on Windows

#### 1. Setup Environment

```powershell
# Verify Flutter
flutter doctor

# Verify Go
go version

# Enable Windows desktop
flutter config --enable-windows-desktop
```

#### 2. Build Application

```powershell
# Option 1: Use build script
cd scripts
.\build_windows.bat

# Option 2: Manual build
cd go_backend
mkdir ..\windows\libs
go build -buildmode=c-shared -o ..\windows\libs\gobackend.dll .
cd ..
flutter build windows --release
```

#### 3. Test Installation

1. Navigate to `build/windows/x64/runner/Release/`
2. Copy `gobackend.dll` from `windows/libs/` to Release folder
3. Run `spotiflac_android.exe`

**Expected Result:**
- App launches successfully
- UI is responsive and properly sized
- All features work (search, download, settings)

### Windows Functional Testing

#### Test Cases:

**1. Application Launch**
- Double-click executable
- App should start within 5 seconds
- No error dialogs

**2. Search Functionality**
- Search for a track
- Results should appear
- Album art should load

**3. Download Functionality**
- Download a track
- Check download location (should be in configured folder)
- Verify file quality and metadata

**4. Proxy Configuration**
- Configure proxy in settings
- Download a track
- Verify proxy is used

**5. Settings Persistence**
- Change settings
- Close app
- Reopen app
- Verify settings are saved

**6. Window Resizing**
- Resize window (minimum ~800x600)
- UI should adapt properly
- No clipping or overflow

**7. Multiple Instances**
- Try opening multiple instances
- Should handle gracefully

### Performance Testing

**Startup Time:**
- Measure time from launch to ready state
- Should be under 5 seconds on modern hardware

**Memory Usage:**
- Monitor RAM usage during operation
- Should stay under 500MB for typical usage

**Download Performance:**
- Compare download speeds with and without proxy
- Should be similar to mobile version

## Common Issues and Solutions

### Proxy Issues

**Issue: "Connection refused"**
- Verify proxy server is running
- Check firewall settings
- Verify proxy host and port are correct

**Issue: "Authentication failed"**
- Check username and password
- Ensure proxy supports the authentication method

**Issue: "SOCKS5 not working"**
- Verify SOCKS5 server is running
- Check if server requires authentication
- Try with HTTP proxy first to isolate issue

### Windows Build Issues

**Issue: "gobackend.dll not found"**
- Ensure you built the Go backend
- Copy DLL to Release folder
- Check for missing dependencies with Dependency Walker

**Issue: "VCRUNTIME140.dll missing"**
- Install Visual C++ Redistributable
- Download from Microsoft website

**Issue: "App won't start"**
- Check Windows Event Viewer for errors
- Run from command line to see error messages
- Verify all DLL dependencies are present

## Automated Testing

Currently, the app relies on manual testing. For future improvements:

### Unit Tests (To Be Added)
- Proxy configuration parsing
- Settings persistence
- URL validation
- Error handling

### Integration Tests (To Be Added)
- End-to-end download flow
- Proxy connection handling
- Platform-specific features

## Reporting Issues

When reporting issues, please include:
1. Platform (Android version or Windows version)
2. Proxy type and configuration (without credentials)
3. Error messages from logs
4. Steps to reproduce
5. Expected vs actual behavior

## Test Environment Examples

### Local Test Setup

**HTTP Proxy with mitmproxy:**
```bash
# Install mitmproxy
pip install mitmproxy

# Run proxy
mitmproxy -p 8080

# Configure in app:
# - Type: HTTP
# - Host: 127.0.0.1
# - Port: 8080
```

**SOCKS5 Proxy with SSH:**
```bash
# Create SSH tunnel
ssh -D 1080 user@remote-server

# Configure in app:
# - Type: SOCKS5
# - Host: 127.0.0.1
# - Port: 1080
```

**HTTP Proxy with Squid:**
```bash
# Install Squid (Ubuntu/Debian)
sudo apt-get install squid

# Configure and start
sudo systemctl start squid

# Configure in app:
# - Type: HTTP
# - Host: 127.0.0.1
# - Port: 3128
```
