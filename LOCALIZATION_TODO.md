# Localization TODO

The following UI strings in the proxy settings feature need to be added to the localization files when updating translations:

## Settings Tab (lib/screens/settings/settings_tab.dart)

Add to localization files (e.g., `lib/l10n/app_en.arb`):

```json
{
  "settingsProxy": "Proxy",
  "settingsProxySubtitle": "Configure HTTP/SOCKS5 proxy"
}
```

Usage in code (line 91-92):
```dart
SettingsItem(
  icon: Icons.vpn_lock_outlined,
  title: l10n.settingsProxy,
  subtitle: l10n.settingsProxySubtitle,
  onTap: () => _navigateTo(context, const ProxySettingsPage()),
  showDivider: false,
),
```

## Proxy Settings Page (lib/screens/settings/proxy_settings_page.dart)

Add to localization files:

```json
{
  "proxySettingsTitle": "Proxy Settings",
  "proxySettingsInfo": "Use a proxy server to route your network traffic. Useful for bypassing ISP blocks or network restrictions.",
  "proxySettingsEnable": "Enable Proxy",
  "proxySettingsEnableSubtitle": "Route traffic through proxy server",
  "proxySettingsType": "Proxy Type",
  "proxySettingsTypeHTTP": "HTTP",
  "proxySettingsTypeHTTPS": "HTTPS",
  "proxySettingsTypeSOCKS5": "SOCKS5",
  "proxySettingsServer": "Proxy Server",
  "proxySettingsHost": "Host",
  "proxySettingsHostHint": "e.g., 127.0.0.1 or proxy.example.com",
  "proxySettingsPort": "Port",
  "proxySettingsPortHint": "e.g., 8080 or 1080",
  "proxySettingsAuth": "Authentication (Optional)",
  "proxySettingsUsername": "Username",
  "proxySettingsPassword": "Password",
  "proxySettingsSave": "Save Settings",
  "proxySettingsSaved": "Proxy settings saved",
  "proxySettingsCommonPorts": "Common Proxy Ports",
  "proxySettingsCommonPortsHint": "• HTTP/HTTPS: 8080, 3128, 8888\n• SOCKS5: 1080, 1081"
}
```

## Implementation Steps

1. Add the above JSON entries to all localization files:
   - `lib/l10n/app_en.arb` (English - main)
   - `lib/l10n/app_*.arb` (other languages)

2. Run the localization generator:
   ```bash
   flutter gen-l10n
   ```

3. Update the code to use localized strings:

   In `settings_tab.dart`:
   ```dart
   title: l10n.settingsProxy,
   subtitle: l10n.settingsProxySubtitle,
   ```

   In `proxy_settings_page.dart`, replace hard-coded strings with:
   ```dart
   appBar: AppBar(
     title: Text(l10n.proxySettingsTitle),
   ),
   // ... and so on for other strings
   ```

4. Test all languages to ensure proper rendering

## Current Status

The proxy feature is fully functional with English hard-coded strings. Adding localization is optional but recommended for consistency with the rest of the app.

## Translation Resources

Use the existing Crowdin project for community translations:
https://crowdin.com/project/spotiflac-mobile

Add these new strings to Crowdin after updating the English source file.
