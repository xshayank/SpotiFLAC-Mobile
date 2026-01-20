import 'dart:io';
import 'package:spotiflac_android/models/settings.dart';
import 'package:spotiflac_android/utils/logger.dart';

final _log = AppLogger('HttpProxy');

/// Configures an HttpClient with proxy settings from AppSettings
/// 
/// This function should be called before making HTTP requests with HttpClient
/// when proxy support is needed.
/// 
/// Example:
/// ```dart
/// final settings = ref.read(settingsProvider);
/// final httpClient = HttpClient();
/// configureHttpClientProxy(httpClient, settings);
/// ```
HttpClient configureHttpClientProxy(HttpClient client, AppSettings settings) {
  if (settings.useProxy && settings.proxyHost.isNotEmpty) {
    final proxyHost = settings.proxyHost;
    final proxyPort = settings.proxyPort;
    final proxyType = settings.proxyType.toLowerCase();
    
    // HttpClient in Dart only supports HTTP/HTTPS proxies, not SOCKS5
    if (proxyType == 'http' || proxyType == 'https') {
      // Build proxy URL for authentication
      String proxyAddress = '$proxyHost:$proxyPort';
      String? proxyAuth;
      
      // Add authentication if provided
      if (settings.proxyUsername.isNotEmpty) {
        final username = Uri.encodeComponent(settings.proxyUsername);
        final password = Uri.encodeComponent(settings.proxyPassword);
        proxyAuth = '$username:$password';
      }
      
      // For Dart's findProxy, format is 'PROXY host:port' without scheme
      client.findProxy = (uri) {
        if (proxyAuth != null) {
          // Note: Dart's HttpClient doesn't support proxy authentication via findProxy
          // Workarounds:
          // 1. Use a local proxy that handles authentication (e.g., Privoxy, mitmproxy)
          // 2. Main API calls use Go backend which supports full authentication
          // 3. For critical downloads, configure proxy at system level
          _log.w('Proxy authentication not supported by Dart HttpClient. '
                 'Use authenticated local proxy or rely on Go backend (API calls) for auth support. '
                 'Cover downloads may fail if proxy requires authentication.');
        }
        return 'PROXY $proxyAddress';
      };
      
      _log.d('Configured HttpClient with proxy: $proxyAddress');
    } else if (proxyType == 'socks5') {
      // SOCKS5 is not supported by Dart's HttpClient
      // The Go backend will handle SOCKS5 proxy for API requests
      // For cover downloads, we'll fallback to direct connection
      _log.w('SOCKS5 proxy not supported for HttpClient (cover downloads). Go backend will use proxy for API requests.');
    }
  }
  
  return client;
}
