#pragma once

#include <string>

/* game modules - obfuscated at runtime */
inline std::string CLIENT_DLL() {
  std::string s = "|l`liu0kii";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

inline std::string ENGINE2_DLL() {
  std::string s = "`l``|l`0kii";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

inline std::string SCHEMASYSTEM_DLL() {
  std::string s = "x|`|^i^v|`|0kii";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

/* game signatures - obfuscated at runtime */
inline std::string GET_SCHEMA_SYSTEM() {
  std::string s = "H)`+`5#? ? ? ? L?+0? ? ? ? 3?`";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

inline std::string GET_ENTITY_LIST() {
  std::string s = "H)+0? ? ? ? H)+|$ ? ? ? ? +i?`1`l";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
}

inline std::string GET_GLOBAL_VARS() {
  std::string s = "H)+5? ? ? ? H)+B";
  for (auto &c : s)
    c ^= 0x1A;
  return s;
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