import 'package:json_annotation/json_annotation.dart';

part 'settings.g.dart';

@JsonSerializable()
class AppSettings {
  final String defaultService;
  final String audioQuality;
  final String filenameFormat;
  final String downloadDirectory;
  final bool autoFallback;
  final bool embedLyrics;
  final bool maxQualityCover;
  final bool isFirstLaunch;
  final int concurrentDownloads;
  final bool checkForUpdates;
  final String updateChannel;
  final bool hasSearchedBefore;
  final String folderOrganization;
  final String historyViewMode;
  final String historyFilterMode;
  final bool askQualityBeforeDownload;
  final String spotifyClientId;
  final String spotifyClientSecret;
  final bool useCustomSpotifyCredentials;
  final String metadataSource;
  final bool enableLogging;
  final bool useExtensionProviders;
  final String? searchProvider;
  final bool separateSingles;
  final String albumFolderStructure;
  final bool showExtensionStore;
  final String locale;
  final bool enableMp3Option;
  final String lyricsMode;
  final bool useProxy;
  final String proxyType;
  final String proxyHost;
  final int proxyPort;
  final String proxyUsername;
  final String proxyPassword;

  const AppSettings({
    this.defaultService = 'tidal',
    this.audioQuality = 'LOSSLESS',
    this.filenameFormat = '{title} - {artist}',
    this.downloadDirectory = '',
    this.autoFallback = true,
    this.embedLyrics = true,
    this.maxQualityCover = true,
    this.isFirstLaunch = true,
    this.concurrentDownloads = 1,
    this.checkForUpdates = true,
    this.updateChannel = 'stable',
    this.hasSearchedBefore = false,
    this.folderOrganization = 'none',
    this.historyViewMode = 'grid',
    this.historyFilterMode = 'all',
    this.askQualityBeforeDownload = true,
    this.spotifyClientId = '',
    this.spotifyClientSecret = '',
    this.useCustomSpotifyCredentials = true,
    this.metadataSource = 'deezer',
    this.enableLogging = false,
    this.useExtensionProviders = true,
    this.searchProvider,
    this.separateSingles = false,
    this.albumFolderStructure = 'artist_album',
    this.showExtensionStore = true,
    this.locale = 'system',
    this.enableMp3Option = false,
    this.lyricsMode = 'embed',
    this.useProxy = false,
    this.proxyType = 'http',
    this.proxyHost = '',
    this.proxyPort = 8080,
    this.proxyUsername = '',
    this.proxyPassword = '',
  });

  AppSettings copyWith({
    String? defaultService,
    String? audioQuality,
    String? filenameFormat,
    String? downloadDirectory,
    bool? autoFallback,
    bool? embedLyrics,
    bool? maxQualityCover,
    bool? isFirstLaunch,
    int? concurrentDownloads,
    bool? checkForUpdates,
    String? updateChannel,
    bool? hasSearchedBefore,
    String? folderOrganization,
    String? historyViewMode,
    String? historyFilterMode,
    bool? askQualityBeforeDownload,
    String? spotifyClientId,
    String? spotifyClientSecret,
    bool? useCustomSpotifyCredentials,
    String? metadataSource,
    bool? enableLogging,
    bool? useExtensionProviders,
    String? searchProvider,
    bool clearSearchProvider = false,
    bool? separateSingles,
    String? albumFolderStructure,
    bool? showExtensionStore,
    String? locale,
    bool? enableMp3Option,
    String? lyricsMode,
    bool? useProxy,
    String? proxyType,
    String? proxyHost,
    int? proxyPort,
    String? proxyUsername,
    String? proxyPassword,
  }) {
    return AppSettings(
      defaultService: defaultService ?? this.defaultService,
      audioQuality: audioQuality ?? this.audioQuality,
      filenameFormat: filenameFormat ?? this.filenameFormat,
      downloadDirectory: downloadDirectory ?? this.downloadDirectory,
      autoFallback: autoFallback ?? this.autoFallback,
      embedLyrics: embedLyrics ?? this.embedLyrics,
      maxQualityCover: maxQualityCover ?? this.maxQualityCover,
      isFirstLaunch: isFirstLaunch ?? this.isFirstLaunch,
      concurrentDownloads: concurrentDownloads ?? this.concurrentDownloads,
      checkForUpdates: checkForUpdates ?? this.checkForUpdates,
      updateChannel: updateChannel ?? this.updateChannel,
      hasSearchedBefore: hasSearchedBefore ?? this.hasSearchedBefore,
      folderOrganization: folderOrganization ?? this.folderOrganization,
      historyViewMode: historyViewMode ?? this.historyViewMode,
      historyFilterMode: historyFilterMode ?? this.historyFilterMode,
      askQualityBeforeDownload: askQualityBeforeDownload ?? this.askQualityBeforeDownload,
      spotifyClientId: spotifyClientId ?? this.spotifyClientId,
      spotifyClientSecret: spotifyClientSecret ?? this.spotifyClientSecret,
      useCustomSpotifyCredentials: useCustomSpotifyCredentials ?? this.useCustomSpotifyCredentials,
      metadataSource: metadataSource ?? this.metadataSource,
      enableLogging: enableLogging ?? this.enableLogging,
      useExtensionProviders: useExtensionProviders ?? this.useExtensionProviders,
      searchProvider: clearSearchProvider ? null : (searchProvider ?? this.searchProvider),
      separateSingles: separateSingles ?? this.separateSingles,
      albumFolderStructure: albumFolderStructure ?? this.albumFolderStructure,
      showExtensionStore: showExtensionStore ?? this.showExtensionStore,
      locale: locale ?? this.locale,
      enableMp3Option: enableMp3Option ?? this.enableMp3Option,
      lyricsMode: lyricsMode ?? this.lyricsMode,
      useProxy: useProxy ?? this.useProxy,
      proxyType: proxyType ?? this.proxyType,
      proxyHost: proxyHost ?? this.proxyHost,
      proxyPort: proxyPort ?? this.proxyPort,
      proxyUsername: proxyUsername ?? this.proxyUsername,
      proxyPassword: proxyPassword ?? this.proxyPassword,
    );
  }

  factory AppSettings.fromJson(Map<String, dynamic> json) =>
      _$AppSettingsFromJson(json);
  Map<String, dynamic> toJson() => _$AppSettingsToJson(this);
}
