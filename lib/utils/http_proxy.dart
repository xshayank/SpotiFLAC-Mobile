import 'dart:io';
import 'package:spotiflac_android/models/settings.dart';

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
      String proxyUrl = '$proxyType://$proxyHost:$proxyPort';
      
      // Add authentication if provided
      if (settings.proxyUsername.isNotEmpty) {
        final username = Uri.encodeComponent(settings.proxyUsername);
        final password = Uri.encodeComponent(settings.proxyPassword);
        proxyUrl = '$proxyType://$username:$password@$proxyHost:$proxyPort';
      }
      
      client.findProxy = (uri) {
        return 'PROXY $proxyUrl';
      };
    } else if (proxyType == 'socks5') {
      // SOCKS5 is not supported by Dart's HttpClient
      // The Go backend will handle SOCKS5 proxy for API requests
      // For cover downloads, we'll fallback to direct connection
      print('Warning: SOCKS5 proxy not supported for HttpClient (cover downloads). Go backend will use proxy for API requests.');
    }
  }
  
  return client;
}
