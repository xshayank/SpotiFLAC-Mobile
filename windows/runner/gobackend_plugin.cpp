#include "gobackend_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>
#include <windows.h>

#include <memory>
#include <string>
#include <iostream>

// Type definitions for Go exported functions
typedef char* (*GoStringFunc)(char*);
typedef char* (*GoStringFunc2)(char*, char*);
typedef char* (*GoStringFunc3)(char*, char*, char*);
typedef char* (*GoStringFunc4)(char*, char*, char*, char*);
typedef char* (*GoStringFunc5)(char*, char*, char*, char*, char*);
typedef char* (*GoStringIntFunc)(char*, long long);
typedef char* (*GoStringIntIntFunc)(char*, long long, long long);
typedef char* (*GoStringIntIntIntFunc)(char*, long long, long long, long long);
typedef void (*GoVoidFunc)();
typedef void (*GoVoidStringFunc)(char*);
typedef void (*GoVoidStringStringFunc)(char*, char*);
typedef void (*GoVoidStringBoolFunc)(char*, unsigned char);
typedef void (*GoVoidBoolFunc)(unsigned char);
typedef void (*GoVoidStringIntFunc)(char*, long long);
typedef void (*GoVoidStringStringStringIntIntFunc)(char*, char*, char*, long long, long long);
typedef unsigned char (*GoBoolFunc)();
typedef long long (*GoIntFunc)();

class GoBackendPlugin {
 public:
  static void RegisterWithRegistrar(flutter::BinaryMessenger* messenger);

  GoBackendPlugin();
  virtual ~GoBackendPlugin();

 private:
  void HandleBackendMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  
  void HandleFFmpegMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  bool LoadGoBackendDLL();
  void UnloadGoBackendDLL();
  
  std::string CallGoStringFunction(const std::string& funcName, const std::string& arg);
  std::string CallGoStringFunction2(const std::string& funcName, const std::string& arg1, const std::string& arg2);
  std::string CallGoStringFunction3(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3);
  std::string CallGoStringFunction4(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4);
  std::string CallGoStringFunction5(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5);
  std::string CallGoStringIntFunction(const std::string& funcName, const std::string& arg, int64_t num);
  std::string CallGoStringIntIntFunction(const std::string& funcName, const std::string& arg, int64_t num1, int64_t num2);
  std::string CallGoStringIntIntIntFunction(const std::string& funcName, const std::string& arg, int64_t num1, int64_t num2, int64_t num3);
  void CallGoVoidFunction(const std::string& funcName);
  void CallGoVoidStringFunction(const std::string& funcName, const std::string& arg);
  void CallGoVoidStringStringFunction(const std::string& funcName, const std::string& arg1, const std::string& arg2);
  void CallGoVoidStringBoolFunction(const std::string& funcName, const std::string& arg, bool value);
  void CallGoVoidBoolFunction(const std::string& funcName, bool value);
  void CallGoVoidStringIntFunction(const std::string& funcName, const std::string& arg, int64_t num);
  void CallGoVoidStringStringStringIntIntFunction(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, int64_t num1, int64_t num2);
  bool CallGoBoolFunction(const std::string& funcName);
  int64_t CallGoIntFunction(const std::string& funcName);

  HMODULE dll_handle_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> backend_channel_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> ffmpeg_channel_;
};

void GoBackendPluginRegisterWithRegistrar(
    flutter::FlutterEngine* engine,
    flutter::BinaryMessenger* messenger) {
  GoBackendPlugin::RegisterWithRegistrar(messenger);
}

void GoBackendPlugin::RegisterWithRegistrar(flutter::BinaryMessenger* messenger) {
  auto plugin = std::make_unique<GoBackendPlugin>();

  plugin->backend_channel_ =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          messenger, "com.zarz.spotiflac/backend",
          &flutter::StandardMethodCodec::GetInstance());

  plugin->backend_channel_->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleBackendMethodCall(call, std::move(result));
      });

  plugin->ffmpeg_channel_ =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          messenger, "com.zarz.spotiflac/ffmpeg",
          &flutter::StandardMethodCodec::GetInstance());

  plugin->ffmpeg_channel_->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleFFmpegMethodCall(call, std::move(result));
      });

  // Keep the plugin alive
  plugin.release();
}

GoBackendPlugin::GoBackendPlugin() : dll_handle_(nullptr) {
  LoadGoBackendDLL();
}

GoBackendPlugin::~GoBackendPlugin() {
  UnloadGoBackendDLL();
}

bool GoBackendPlugin::LoadGoBackendDLL() {
  // Try to load gobackend.dll from the same directory as the executable
  dll_handle_ = LoadLibraryA("gobackend.dll");
  if (!dll_handle_) {
    std::cerr << "Failed to load gobackend.dll. Error: " << GetLastError() << std::endl;
    return false;
  }
  std::cout << "Successfully loaded gobackend.dll" << std::endl;
  return true;
}

void GoBackendPlugin::UnloadGoBackendDLL() {
  if (dll_handle_) {
    FreeLibrary(dll_handle_);
    dll_handle_ = nullptr;
  }
}

std::string GoBackendPlugin::CallGoStringFunction(const std::string& funcName, const std::string& arg) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  char* result = func(input);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result); // Free Go-allocated memory
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringFunction2(const std::string& funcName, const std::string& arg1, const std::string& arg2) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringFunc2>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  char* result = func(input1, input2);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringFunction3(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringFunc3>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  char* input3 = const_cast<char*>(arg3.c_str());
  char* result = func(input1, input2, input3);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringFunction4(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringFunc4>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  char* input3 = const_cast<char*>(arg3.c_str());
  char* input4 = const_cast<char*>(arg4.c_str());
  char* result = func(input1, input2, input3, input4);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringFunction5(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringFunc5>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  char* input3 = const_cast<char*>(arg3.c_str());
  char* input4 = const_cast<char*>(arg4.c_str());
  char* input5 = const_cast<char*>(arg5.c_str());
  char* result = func(input1, input2, input3, input4, input5);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringIntFunction(const std::string& funcName, const std::string& arg, int64_t num) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  char* result = func(input, num);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringIntIntFunction(const std::string& funcName, const std::string& arg, int64_t num1, int64_t num2) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringIntIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  char* result = func(input, num1, num2);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

std::string GoBackendPlugin::CallGoStringIntIntIntFunction(const std::string& funcName, const std::string& arg, int64_t num1, int64_t num2, int64_t num3) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoStringIntIntIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  char* result = func(input, num1, num2, num3);
  
  if (!result) {
    return "";
  }
  
  std::string resultStr(result);
  free(result);
  return resultStr;
}

void GoBackendPlugin::CallGoVoidFunction(const std::string& funcName) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  func();
}

void GoBackendPlugin::CallGoVoidStringFunction(const std::string& funcName, const std::string& arg) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidStringFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  func(input);
}

void GoBackendPlugin::CallGoVoidStringStringFunction(const std::string& funcName, const std::string& arg1, const std::string& arg2) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidStringStringFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  func(input1, input2);
}

void GoBackendPlugin::CallGoVoidStringBoolFunction(const std::string& funcName, const std::string& arg, bool value) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidStringBoolFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  func(input, value ? 1 : 0);
}

void GoBackendPlugin::CallGoVoidBoolFunction(const std::string& funcName, bool value) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidBoolFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  func(value ? 1 : 0);
}

void GoBackendPlugin::CallGoVoidStringIntFunction(const std::string& funcName, const std::string& arg, int64_t num) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidStringIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input = const_cast<char*>(arg.c_str());
  func(input, num);
}

void GoBackendPlugin::CallGoVoidStringStringStringIntIntFunction(const std::string& funcName, const std::string& arg1, const std::string& arg2, const std::string& arg3, int64_t num1, int64_t num2) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoVoidStringStringStringIntIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  char* input1 = const_cast<char*>(arg1.c_str());
  char* input2 = const_cast<char*>(arg2.c_str());
  char* input3 = const_cast<char*>(arg3.c_str());
  func(input1, input2, input3, num1, num2);
}

bool GoBackendPlugin::CallGoBoolFunction(const std::string& funcName) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoBoolFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  return func() != 0;
}

int64_t GoBackendPlugin::CallGoIntFunction(const std::string& funcName) {
  if (!dll_handle_) {
    throw std::runtime_error("Go backend DLL not loaded");
  }
  
  auto func = reinterpret_cast<GoIntFunc>(GetProcAddress(dll_handle_, funcName.c_str()));
  if (!func) {
    throw std::runtime_error("Function not found: " + funcName);
  }
  
  return func();
}

// Helper function to get string argument
std::string GetStringArg(const flutter::EncodableValue* args, const std::string& key, const std::string& defaultValue = "") {
  if (!args || !std::holds_alternative<flutter::EncodableMap>(*args)) {
    return defaultValue;
  }
  
  const auto& map = std::get<flutter::EncodableMap>(*args);
  auto it = map.find(flutter::EncodableValue(key));
  if (it != map.end() && std::holds_alternative<std::string>(it->second)) {
    return std::get<std::string>(it->second);
  }
  return defaultValue;
}

// Helper function to get int argument
int64_t GetIntArg(const flutter::EncodableValue* args, const std::string& key, int64_t defaultValue = 0) {
  if (!args || !std::holds_alternative<flutter::EncodableMap>(*args)) {
    return defaultValue;
  }
  
  const auto& map = std::get<flutter::EncodableMap>(*args);
  auto it = map.find(flutter::EncodableValue(key));
  if (it != map.end()) {
    if (std::holds_alternative<int32_t>(it->second)) {
      return std::get<int32_t>(it->second);
    } else if (std::holds_alternative<int64_t>(it->second)) {
      return std::get<int64_t>(it->second);
    }
  }
  return defaultValue;
}

// Helper function to get bool argument
bool GetBoolArg(const flutter::EncodableValue* args, const std::string& key, bool defaultValue = false) {
  if (!args || !std::holds_alternative<flutter::EncodableMap>(*args)) {
    return defaultValue;
  }
  
  const auto& map = std::get<flutter::EncodableMap>(*args);
  auto it = map.find(flutter::EncodableValue(key));
  if (it != map.end() && std::holds_alternative<bool>(it->second)) {
    return std::get<bool>(it->second);
  }
  return defaultValue;
}

void GoBackendPlugin::HandleBackendMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  
  const std::string& method = method_call.method_name();
  const auto* args = method_call.arguments();
  
  try {
    // Parse methods - single string argument
    if (method == "parseSpotifyUrl") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("ParseSpotifyURL", url);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "parseDeezerUrl") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("ParseDeezerURLExport", url);
      result->Success(flutter::EncodableValue(response));
    }
    // Metadata methods
    else if (method == "getSpotifyMetadata") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("GetSpotifyMetadata", url);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getSpotifyMetadataWithFallback") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("GetSpotifyMetadataWithDeezerFallback", url);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getDeezerMetadata") {
      std::string resourceType = GetStringArg(args, "resource_type");
      std::string resourceId = GetStringArg(args, "resource_id");
      std::string response = CallGoStringFunction2("GetDeezerMetadata", resourceType, resourceId);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getDeezerExtendedMetadata") {
      std::string trackId = GetStringArg(args, "track_id");
      std::string response = CallGoStringFunction("GetDeezerExtendedMetadata", trackId);
      result->Success(flutter::EncodableValue(response));
    }
    // Search methods
    else if (method == "searchSpotify") {
      std::string query = GetStringArg(args, "query");
      int64_t limit = GetIntArg(args, "limit", 10);
      std::string response = CallGoStringIntFunction("SearchSpotify", query, limit);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "searchSpotifyAll") {
      std::string query = GetStringArg(args, "query");
      int64_t trackLimit = GetIntArg(args, "track_limit", 15);
      int64_t artistLimit = GetIntArg(args, "artist_limit", 3);
      std::string response = CallGoStringIntIntFunction("SearchSpotifyAll", query, trackLimit, artistLimit);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "searchDeezerAll") {
      std::string query = GetStringArg(args, "query");
      int64_t trackLimit = GetIntArg(args, "track_limit", 15);
      int64_t artistLimit = GetIntArg(args, "artist_limit", 3);
      std::string response = CallGoStringIntIntFunction("SearchDeezerAll", query, trackLimit, artistLimit);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "searchDeezerByISRC") {
      std::string isrc = GetStringArg(args, "isrc");
      std::string response = CallGoStringFunction("SearchDeezerByISRC", isrc);
      result->Success(flutter::EncodableValue(response));
    }
    // Availability check
    else if (method == "checkAvailability") {
      std::string spotifyId = GetStringArg(args, "spotify_id");
      std::string isrc = GetStringArg(args, "isrc");
      std::string response = CallGoStringFunction2("CheckAvailability", spotifyId, isrc);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "convertSpotifyToDeezer") {
      std::string resourceType = GetStringArg(args, "resource_type");
      std::string spotifyId = GetStringArg(args, "spotify_id");
      std::string response = CallGoStringFunction2("ConvertSpotifyToDeezer", resourceType, spotifyId);
      result->Success(flutter::EncodableValue(response));
    }
    // Download methods
    else if (method == "downloadTrack") {
      if (args && std::holds_alternative<std::string>(*args)) {
        std::string requestJson = std::get<std::string>(*args);
        std::string response = CallGoStringFunction("DownloadTrack", requestJson);
        result->Success(flutter::EncodableValue(response));
      } else {
        result->Error("INVALID_ARGUMENT", "Expected JSON string");
      }
    }
    else if (method == "downloadWithFallback") {
      if (args && std::holds_alternative<std::string>(*args)) {
        std::string requestJson = std::get<std::string>(*args);
        std::string response = CallGoStringFunction("DownloadWithFallback", requestJson);
        result->Success(flutter::EncodableValue(response));
      } else {
        result->Error("INVALID_ARGUMENT", "Expected JSON string");
      }
    }
    else if (method == "downloadWithExtensions") {
      if (args && std::holds_alternative<std::string>(*args)) {
        std::string requestJson = std::get<std::string>(*args);
        std::string response = CallGoStringFunction("DownloadWithExtensionsJSON", requestJson);
        result->Success(flutter::EncodableValue(response));
      } else {
        result->Error("INVALID_ARGUMENT", "Expected JSON string");
      }
    }
    // Progress methods
    else if (method == "getDownloadProgress") {
      std::string response = CallGoStringFunction("GetDownloadProgress", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getAllDownloadProgress") {
      std::string response = CallGoStringFunction("GetAllDownloadProgress", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "initItemProgress") {
      std::string itemId = GetStringArg(args, "item_id");
      CallGoVoidStringFunction("InitItemProgress", itemId);
      result->Success();
    }
    else if (method == "finishItemProgress") {
      std::string itemId = GetStringArg(args, "item_id");
      CallGoVoidStringFunction("FinishItemProgress", itemId);
      result->Success();
    }
    else if (method == "clearItemProgress") {
      std::string itemId = GetStringArg(args, "item_id");
      CallGoVoidStringFunction("ClearItemProgress", itemId);
      result->Success();
    }
    else if (method == "cancelDownload") {
      std::string itemId = GetStringArg(args, "item_id");
      CallGoVoidStringFunction("CancelDownload", itemId);
      result->Success();
    }
    // Directory and file methods
    else if (method == "setDownloadDirectory") {
      std::string path = GetStringArg(args, "path");
      CallGoVoidStringFunction("SetDownloadDirectory", path);
      result->Success();
    }
    else if (method == "checkDuplicate") {
      std::string outputDir = GetStringArg(args, "output_dir");
      std::string isrc = GetStringArg(args, "isrc");
      std::string response = CallGoStringFunction2("CheckDuplicate", outputDir, isrc);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "buildFilename") {
      std::string templateStr = GetStringArg(args, "template");
      std::string metadata = GetStringArg(args, "metadata", "{}");
      std::string response = CallGoStringFunction2("BuildFilename", templateStr, metadata);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "sanitizeFilename") {
      std::string filename = GetStringArg(args, "filename");
      std::string response = CallGoStringFunction("SanitizeFilename", filename);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "readFileMetadata") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string response = CallGoStringFunction("ReadFileMetadata", filePath);
      result->Success(flutter::EncodableValue(response));
    }
    // Lyrics methods
    else if (method == "fetchLyrics") {
      std::string spotifyId = GetStringArg(args, "spotify_id");
      std::string trackName = GetStringArg(args, "track_name");
      std::string artistName = GetStringArg(args, "artist_name");
      int64_t durationMs = GetIntArg(args, "duration_ms");
      std::string response = CallGoStringIntFunction("FetchLyrics", spotifyId, durationMs);
      // Note: FetchLyrics in Go takes spotifyId, trackName, artistName, durationMs
      // We need to adapt this
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getLyricsLRC") {
      std::string spotifyId = GetStringArg(args, "spotify_id");
      std::string trackName = GetStringArg(args, "track_name");
      std::string artistName = GetStringArg(args, "artist_name");
      std::string filePath = GetStringArg(args, "file_path");
      int64_t durationMs = GetIntArg(args, "duration_ms");
      std::string response = CallGoStringFunction("GetLyricsLRC", spotifyId);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "embedLyricsToFile") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string lyrics = GetStringArg(args, "lyrics");
      std::string response = CallGoStringFunction2("EmbedLyricsToFile", filePath, lyrics);
      result->Success(flutter::EncodableValue(response));
    }
    // Cleanup
    else if (method == "cleanupConnections") {
      CallGoVoidFunction("CleanupConnections");
      result->Success();
    }
    // Service methods (Windows-specific stubs for now)
    else if (method == "startDownloadService" || 
             method == "stopDownloadService" || 
             method == "updateDownloadServiceProgress" ||
             method == "isDownloadServiceRunning") {
      // These are Android-specific service methods - stub on Windows
      if (method == "isDownloadServiceRunning") {
        result->Success(flutter::EncodableValue(false));
      } else {
        result->Success();
      }
    }
    // Credentials
    else if (method == "setSpotifyCredentials") {
      std::string clientId = GetStringArg(args, "client_id");
      std::string clientSecret = GetStringArg(args, "client_secret");
      CallGoVoidStringStringFunction("SetSpotifyAPICredentials", clientId, clientSecret);
      result->Success();
    }
    else if (method == "hasSpotifyCredentials") {
      bool hasCredentials = CallGoBoolFunction("CheckSpotifyCredentials");
      result->Success(flutter::EncodableValue(hasCredentials));
    }
    // Cache methods
    else if (method == "preWarmTrackCache") {
      std::string tracksJson = GetStringArg(args, "tracks", "[]");
      std::string response = CallGoStringFunction("PreWarmTrackCacheJSON", tracksJson);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getTrackCacheSize") {
      int64_t size = CallGoIntFunction("GetTrackCacheSize");
      result->Success(flutter::EncodableValue(static_cast<int32_t>(size)));
    }
    else if (method == "clearTrackCache") {
      CallGoVoidFunction("ClearTrackIDCache");
      result->Success();
    }
    // Log methods
    else if (method == "getLogs") {
      std::string response = CallGoStringFunction("GetLogs", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getLogsSince") {
      int64_t index = GetIntArg(args, "index");
      std::string response = CallGoStringIntFunction("GetLogsSince", "", index);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "clearLogs") {
      CallGoVoidFunction("ClearLogs");
      result->Success();
    }
    else if (method == "getLogCount") {
      int64_t count = CallGoIntFunction("GetLogCount");
      result->Success(flutter::EncodableValue(static_cast<int32_t>(count)));
    }
    else if (method == "setLoggingEnabled") {
      bool enabled = GetBoolArg(args, "enabled");
      CallGoVoidBoolFunction("SetLoggingEnabled", enabled);
      result->Success();
    }
    // Extension System methods
    else if (method == "initExtensionSystem") {
      std::string extensionsDir = GetStringArg(args, "extensions_dir");
      std::string dataDir = GetStringArg(args, "data_dir");
      CallGoVoidStringStringFunction("InitExtensionSystem", extensionsDir, dataDir);
      result->Success();
    }
    else if (method == "loadExtensionsFromDir") {
      std::string dirPath = GetStringArg(args, "dir_path");
      std::string response = CallGoStringFunction("LoadExtensionsFromDir", dirPath);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "loadExtensionFromPath") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string response = CallGoStringFunction("LoadExtensionFromPath", filePath);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "unloadExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      CallGoVoidStringFunction("UnloadExtensionByID", extensionId);
      result->Success();
    }
    else if (method == "removeExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      CallGoVoidStringFunction("RemoveExtensionByID", extensionId);
      result->Success();
    }
    else if (method == "upgradeExtension") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string response = CallGoStringFunction("UpgradeExtensionFromPath", filePath);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "checkExtensionUpgrade") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string response = CallGoStringFunction("CheckExtensionUpgradeFromPath", filePath);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getInstalledExtensions") {
      std::string response = CallGoStringFunction("GetInstalledExtensions", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "setExtensionEnabled") {
      std::string extensionId = GetStringArg(args, "extension_id");
      bool enabled = GetBoolArg(args, "enabled");
      CallGoVoidStringBoolFunction("SetExtensionEnabledByID", extensionId, enabled);
      result->Success();
    }
    else if (method == "setProviderPriority") {
      std::string priorityJson = GetStringArg(args, "priority", "[]");
      CallGoVoidStringFunction("SetProviderPriorityJSON", priorityJson);
      result->Success();
    }
    else if (method == "getProviderPriority") {
      std::string response = CallGoStringFunction("GetProviderPriorityJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "setMetadataProviderPriority") {
      std::string priorityJson = GetStringArg(args, "priority", "[]");
      CallGoVoidStringFunction("SetMetadataProviderPriorityJSON", priorityJson);
      result->Success();
    }
    else if (method == "getMetadataProviderPriority") {
      std::string response = CallGoStringFunction("GetMetadataProviderPriorityJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getExtensionSettings") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string response = CallGoStringFunction("GetExtensionSettingsJSON", extensionId);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "setExtensionSettings") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string settingsJson = GetStringArg(args, "settings", "{}");
      CallGoVoidStringStringFunction("SetExtensionSettingsJSON", extensionId, settingsJson);
      result->Success();
    }
    else if (method == "invokeExtensionAction") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string actionName = GetStringArg(args, "action");
      std::string response = CallGoStringFunction2("InvokeExtensionActionJSON", extensionId, actionName);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "searchTracksWithExtensions") {
      std::string query = GetStringArg(args, "query");
      int64_t limit = GetIntArg(args, "limit", 20);
      std::string response = CallGoStringIntFunction("SearchTracksWithExtensionsJSON", query, limit);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "cleanupExtensions") {
      CallGoVoidFunction("CleanupExtensions");
      result->Success();
    }
    // Extension Auth API methods
    else if (method == "getExtensionPendingAuth") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string response = CallGoStringFunction("GetExtensionPendingAuthJSON", extensionId);
      if (response.empty()) {
        result->Success();
      } else {
        result->Success(flutter::EncodableValue(response));
      }
    }
    else if (method == "setExtensionAuthCode") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string authCode = GetStringArg(args, "auth_code");
      CallGoVoidStringStringFunction("SetExtensionAuthCodeByID", extensionId, authCode);
      result->Success();
    }
    else if (method == "setExtensionTokens") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string accessToken = GetStringArg(args, "access_token");
      std::string refreshToken = GetStringArg(args, "refresh_token");
      int64_t expiresIn = GetIntArg(args, "expires_in");
      CallGoVoidStringStringStringIntIntFunction("SetExtensionTokensByID", extensionId, accessToken, refreshToken, expiresIn, 0);
      result->Success();
    }
    else if (method == "clearExtensionPendingAuth") {
      std::string extensionId = GetStringArg(args, "extension_id");
      CallGoVoidStringFunction("ClearExtensionPendingAuthByID", extensionId);
      result->Success();
    }
    else if (method == "isExtensionAuthenticated") {
      std::string extensionId = GetStringArg(args, "extension_id");
      bool isAuth = CallGoBoolFunction("IsExtensionAuthenticatedByID");
      result->Success(flutter::EncodableValue(isAuth));
    }
    else if (method == "getAllPendingAuthRequests") {
      std::string response = CallGoStringFunction("GetAllPendingAuthRequestsJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    // Extension FFmpeg API
    else if (method == "getPendingFFmpegCommand") {
      std::string commandId = GetStringArg(args, "command_id");
      std::string response = CallGoStringFunction("GetPendingFFmpegCommandJSON", commandId);
      if (response.empty()) {
        result->Success();
      } else {
        result->Success(flutter::EncodableValue(response));
      }
    }
    else if (method == "setFFmpegCommandResult") {
      std::string commandId = GetStringArg(args, "command_id");
      bool success = GetBoolArg(args, "success");
      std::string output = GetStringArg(args, "output");
      std::string error = GetStringArg(args, "error");
      CallGoVoidStringFunction("SetFFmpegCommandResultByID", commandId);
      result->Success();
    }
    else if (method == "getAllPendingFFmpegCommands") {
      std::string response = CallGoStringFunction("GetAllPendingFFmpegCommandsJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    // Extension Custom Search API
    else if (method == "customSearchWithExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string query = GetStringArg(args, "query");
      std::string optionsJson = GetStringArg(args, "options", "");
      std::string response = CallGoStringFunction3("CustomSearchWithExtensionJSON", extensionId, query, optionsJson);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getSearchProviders") {
      std::string response = CallGoStringFunction("GetSearchProvidersJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    // Extension URL Handler API
    else if (method == "handleURLWithExtension") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("HandleURLWithExtensionJSON", url);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "findURLHandler") {
      std::string url = GetStringArg(args, "url");
      std::string response = CallGoStringFunction("FindURLHandlerJSON", url);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getURLHandlers") {
      std::string response = CallGoStringFunction("GetURLHandlersJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getAlbumWithExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string albumId = GetStringArg(args, "album_id");
      std::string response = CallGoStringFunction2("GetAlbumWithExtensionJSON", extensionId, albumId);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getPlaylistWithExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string playlistId = GetStringArg(args, "playlist_id");
      std::string response = CallGoStringFunction2("GetPlaylistWithExtensionJSON", extensionId, playlistId);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getArtistWithExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string artistId = GetStringArg(args, "artist_id");
      std::string response = CallGoStringFunction2("GetArtistWithExtensionJSON", extensionId, artistId);
      result->Success(flutter::EncodableValue(response));
    }
    // Extension Post-Processing API
    else if (method == "runPostProcessing") {
      std::string filePath = GetStringArg(args, "file_path");
      std::string metadataJson = GetStringArg(args, "metadata", "");
      std::string response = CallGoStringFunction2("RunPostProcessingJSON", filePath, metadataJson);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getPostProcessingProviders") {
      std::string response = CallGoStringFunction("GetPostProcessingProvidersJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    // Extension Store
    else if (method == "initExtensionStore") {
      std::string cacheDir = GetStringArg(args, "cache_dir");
      CallGoVoidStringFunction("InitExtensionStoreJSON", cacheDir);
      result->Success();
    }
    else if (method == "getStoreExtensions") {
      bool forceRefresh = GetBoolArg(args, "force_refresh");
      std::string response = CallGoStringFunction("GetStoreExtensionsJSON", forceRefresh ? "true" : "false");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "searchStoreExtensions") {
      std::string query = GetStringArg(args, "query");
      std::string category = GetStringArg(args, "category");
      std::string response = CallGoStringFunction2("SearchStoreExtensionsJSON", query, category);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "getStoreCategories") {
      std::string response = CallGoStringFunction("GetStoreCategoriesJSON", "");
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "downloadStoreExtension") {
      std::string extensionId = GetStringArg(args, "extension_id");
      std::string destDir = GetStringArg(args, "dest_dir");
      std::string response = CallGoStringFunction2("DownloadStoreExtensionJSON", extensionId, destDir);
      result->Success(flutter::EncodableValue(response));
    }
    else if (method == "clearStoreCache") {
      CallGoVoidFunction("ClearStoreCacheJSON");
      result->Success();
    }
    // Proxy Configuration
    else if (method == "setProxyConfig") {
      std::string proxyType = GetStringArg(args, "proxy_type");
      std::string host = GetStringArg(args, "host");
      int64_t port = GetIntArg(args, "port");
      std::string username = GetStringArg(args, "username");
      std::string password = GetStringArg(args, "password");
      CallGoVoidStringStringStringIntIntFunction("SetProxyConfigJSON", proxyType, host, username, port, 0);
      result->Success();
    }
    else if (method == "clearProxyConfig") {
      CallGoVoidFunction("ClearProxyConfigJSON");
      result->Success();
    }
    else {
      result->NotImplemented();
    }
  } catch (const std::exception& e) {
    result->Error("ERROR", e.what());
  }
}

void GoBackendPlugin::HandleFFmpegMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  
  const std::string& method = method_call.method_name();
  
  // FFmpeg is not yet implemented on Windows
  if (method == "execute") {
    flutter::EncodableMap errorMap;
    errorMap[flutter::EncodableValue("success")] = flutter::EncodableValue(false);
    errorMap[flutter::EncodableValue("returnCode")] = flutter::EncodableValue(-1);
    errorMap[flutter::EncodableValue("output")] = flutter::EncodableValue("FFmpeg is not yet implemented on Windows. Please use the extension system for audio conversion.");
    result->Success(flutter::EncodableValue(errorMap));
  } else if (method == "getVersion") {
    result->Success(flutter::EncodableValue("FFmpeg not available on Windows"));
  } else {
    result->NotImplemented();
  }
}
