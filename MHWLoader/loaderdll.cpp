// dllmain.cpp : Defines the entry point for the DLL application.

#include <MemoryModule.h>
#include <MinHook.h>
#include <SDKDDKVer.h>
#include <TlHelp32.h>
#include <windows.h>
#include <winternl.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "Threader.h"
#include "dll.h"
#include "ghidra_export.h"
#include "loader.h"
#include "memory_patch.h"

using namespace loader;

namespace fs = std::filesystem;
using FilePath = fs::path;
using FileTime = fs::file_time_type;
using DLL = std::tuple<HMEMORYMODULE, FilePath, FileTime>;

std::vector<DLL> dlls;
Threader threader;

const char* loader::GameVersion = "???";

static void* currentModule;

HCUSTOMMODULE CustomLoadLibrary(const char* path, void* _) {
  if (std::string(path) == "loader.dll") {
    return currentModule;
  }
  return LoadLibraryA(path);
}

FARPROC CustomGetProcAddress(HCUSTOMMODULE target, const char* path, void* _) {
  if (target == currentModule) {
    return MemoryGetProcAddress(target, path);
  }
  return GetProcAddress((HMODULE)target, path);
}

auto LoadDll(const char* path) {
  std::ifstream dll(path, std::ios::binary);
  std::vector<char> dllRead(std::istreambuf_iterator<char>(dll), {});

  size_t size = dllRead.size();
  char* allocatedMem = (char*)malloc(size);
  memcpy(allocatedMem, &dllRead[0], size);

  return MemoryLoadLibraryEx(allocatedMem, size, MemoryDefaultAlloc,
                             MemoryDefaultFree, CustomLoadLibrary,
                             CustomGetProcAddress, MemoryDefaultFreeLibrary,
                             nullptr);
}

void LoadAllPluginDlls() {
  if (!ConfigFile.value("enablePluginLoader", true)) return;
  if (!std::filesystem::exists("nativePC\\plugins")) return;
  for (auto& entry : std::filesystem::directory_iterator("nativePC\\plugins")) {
    std::string name = entry.path().filename().string();
    if (entry.path().filename().extension().string() != ".dll") continue;
    LOG(INFO) << "Loading plugin " << entry.path();
    auto dll = LoadDll(entry.path().string().c_str());
    if (!dll) {
      LOG(ERR) << "Failed to load " << entry.path();
    } else {
      dlls.push_back({dll, entry.path(), entry.last_write_time()});
    }
  }
}

void OldWarning() {
  if (std::filesystem::exists("hid.dll")) {
    LOG(ERR) << "Found old hid.dll in game folder.";
    LOG(ERR)
        << "This dll is not used by the mod anymore, and might cause issues.";
    LOG(ERR) << "Please delete hid.dll from the game's folder.";
  }
}

static std::string GameVersionString;

void FindVersion() {
  //   const auto CalcNumBits =
  //       "01001... 10001101 01001100 ..100100 00110000 01001... 10001101"
  //       "01000010 11110101";
  //   auto [binData, binMask] = parseBinary(CalcNumBits);
  //   auto found = scanmem(binData, binMask);
  //   if (found.size() != 1) {
  //     LOG(ERR) << "Build Number check failed.";
  //     LOG(ERR) << "Could not find game version";
  //     LOG(ERR) << "Loader needs to be updated.";
  //     return;
  //   }

  //   LOG(INFO) << std::hex << "GameVersion AOB found at: 0x"
  //             << (uintptr_t)found[0];

  //   byte* instruction = found[0] - 0x18;

  auto instruction = (byte*)(0x1418e61ec - 0x18);

  char** exeGameVersion =
      (char**)(instruction + *(uint32_t*)(instruction + 0x3) + 0x7);
  GameVersionString = *exeGameVersion;
  GameVersion = GameVersionString.c_str();
}

void liveReloading() {
  for (auto& dll : dlls) {
    auto filePath = std::get<FilePath>(dll);
    auto writeTime = fs::last_write_time(filePath);

    if (writeTime != std::get<FileTime>(dll)) {
      LOG(INFO) << "Reloading " << filePath;
      MemoryFreeLibrary(std::get<HMEMORYMODULE>(dll));
      auto memModule = LoadDll(filePath.string().c_str());

      if (!memModule) {
        LOG(ERR) << "Failed to load " << filePath;
      }
      std::get<HMEMORYMODULE>(dll) = memModule;
      std::get<FileTime>(dll) = writeTime;
    }
  }
}

void loop() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    liveReloading();
  }
}

extern "C" {
__declspec(dllexport) extern void Initialize(void* memModule) {
  currentModule = memModule;
  try {
    LoadConfig();
    OldWarning();
    FindVersion();
    LoadAllPluginDlls();

    threader.startThread(loop);

    return;
  } catch (std::exception e) {
    MessageBox(0, "STRACKER'S LOADER ERROR", e.what(), MB_OK);
  }
}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  return TRUE;
}
