#pragma once

#include <string>

/* game modules - obfuscated at runtime */
inline std::string CLIENT_DLL() { return "client.dll"; }

inline std::string ENGINE2_DLL() { return "engine2.dll"; }

inline std::string SCHEMASYSTEM_DLL() { return "schemasystem.dll"; }

/* game signatures - temporary plaintext for debugging */
inline std::string GET_SCHEMA_SYSTEM() {
  return "48 8B 05 ? ? ? ? 48 8B D9 48 85 C0 74 1D 48 8B 40 30 48 8B 00";
}

inline std::string GET_ENTITY_LIST() {
  return "48 8B 05 ? ? ? ? 48 8B 50 08 48 85 C0 74 0D";
}

inline std::string GET_GLOBAL_VARS() {
  return "48 8B 05 ? ? ? ? 48 8B 40 08 48 85 C0 74 0D";
}

inline std::string GET_LOCAL_PLAYER_CONTROLLER() {
  std::string s = "L?+0? ? ? ? 3?2 L?+?`?";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

/* custom defines */
#define LOG_INFO(str, ...) printf(" [info] " str "\n", __VA_ARGS__)

#define LOG_WARNING(str, ...) printf(" [warning] " str "\n", __VA_ARGS__)

#define LOG_ERROR(str, ...)                                                    \
  {                                                                            \
    const auto filename = std::filesystem::path(__FILE__).filename().string(); \
    printf(" [error] [%s:%d] " str "\n", filename.c_str(), __LINE__,           \
           __VA_ARGS__);                                                       \
  }

#define INIT_STEP(name, expr)                                                  \
  if (!(expr)) {                                                               \
    std::this_thread::sleep_for(std::chrono::seconds(5));                      \
    return {};                                                                 \
  }                                                                            \
  LOG_INFO(name " initialization completed")