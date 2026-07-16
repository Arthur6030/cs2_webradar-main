#include "pch.hpp"

bool i::setup() {
  bool success = true;

  printf("[debug] getting client module info...\n");
  auto client_name = CLIENT_DLL();
  printf("[debug] looking for client module: %s\n", client_name.c_str());
  const auto [client_base, client_size] =
      m_memory->get_module_info(client_name);
  if (!client_base.has_value() || !client_size.has_value()) {
    LOG_ERROR("failed to get client module info");
    return false;
  }
  printf("[debug] client base: 0x%llx, size: 0x%llx\n", client_base.value(),
         client_size.value());

  printf("[warning] skipping pattern-based setup for now\n");
  m_schema_system = nullptr;
  m_global_vars = nullptr;
  m_game_entity_system = nullptr;
  success = true;

  return success;
}
