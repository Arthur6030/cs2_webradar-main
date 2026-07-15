#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <winternl.h>

namespace antiban {
inline std::mt19937 &get_rng() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return gen;
}

// Random sleep between min_ms and max_ms to avoid timing patterns
inline void random_sleep(int min_ms = 30, int max_ms = 150) {
  std::uniform_int_distribution<int> dist(min_ms, max_ms);
  std::this_thread::sleep_for(std::chrono::milliseconds(dist(get_rng())));
}

// Obfuscate a string at compile time (simple XOR)
inline std::string obfuscate(const std::string &str, char key = 0xAB) {
  std::string result = str;
  for (auto &c : result)
    c ^= key;
  return result;
}

// Deobfuscate
inline std::string deobfuscate(const std::string &str, char key = 0xAB) {
  return obfuscate(str, key); // XOR is symmetric
}

// Add random junk code execution to confuse scanners
inline void junk_code() {
  volatile int junk = rand() % 100;
  volatile int junk2 = junk * junk;
  volatile int junk3 = junk2 / (junk + 1);
  (void)junk3;
}

// Check if being debugged
inline bool is_debugged() { return IsDebuggerPresent() != 0; }
} // namespace antiban