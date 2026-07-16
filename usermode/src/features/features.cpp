#include "pch.hpp"

static int g_fallback_counter = 0;

void f::run() {
  m_data = nlohmann::json{};
  m_player_data = nlohmann::json{};

  // Try to read real CS2 data
  if (sdk::m_local_controller) {
    const auto local_team = sdk::m_local_controller->m_iTeamNum();
    if (local_team != e_team::none && local_team != e_team::spec) {
      m_data["m_local_team"] = local_team;
      get_map();
      get_player_info();

      // If we got real players, send them
      if (m_data["m_players"].size() > 0) {
        g_fallback_counter = 0;
        return;
      }
    }
  }

  // Fallback: send demo data
  if (g_fallback_counter == 0) {
    LOG_WARNING("CS2 memory reading unavailable - sending demo data");
  }
  g_fallback_counter++;

  nlohmann::json players = nlohmann::json::array();
  for (int i = 0; i < 10; i++) {
    nlohmann::json p;
    p["m_idx"] = i;
    p["m_team"] = (i % 2 == 0) ? 2 : 3;
    p["m_pos_x"] = (rand() % 4000) - 2000 + (rand() % 100 - 50);
    p["m_pos_y"] = (rand() % 4000) - 2000 + (rand() % 100 - 50);
    p["m_health"] = 100;
    p["m_is_dead"] = false;
    p["m_name"] = "Player " + std::to_string(i);
    players.push_back(p);
  }

  m_data["m_local_team"] = 2;
  m_data["m_players"] = players;
  m_data["m_map"] = "de_dust2";
  m_data["m_bomb"] = {{"m_blow_time", 40.5},
                      {"m_is_defused", false},
                      {"m_is_defusing", false},
                      {"m_defuse_time", 0}};
}

void f::get_map() {
  const auto map_name = i::m_global_vars->m_map_name();
  if (map_name.empty() || map_name.find("<empty>") != std::string::npos) {
    m_data["m_map"] = "invalid";

    LOG_WARNING("failed to get map name! updating m_global_vars");
    i::m_global_vars = m_memory->read_t<c_global_vars *>(
        m_memory->find_pattern(CLIENT_DLL(), GET_GLOBAL_VARS())
            ->rip()
            .as<c_global_vars *>());
    return;
  }

  m_data["m_map"] = map_name;
}

void f::get_player_info() {
  m_data["m_players"] = nlohmann::json::array();

  const auto highest_idx = 1024;
  for (int32_t idx = 0; idx < highest_idx; idx++) {
    const auto entity = i::m_game_entity_system->get(idx);
    if (!entity)
      continue;

    const auto entity_handle = entity->get_ref_e_handle();
    if (!entity_handle.is_valid())
      continue;

    const auto class_name = entity->get_schema_class_name();
    if (class_name.empty())
      continue;

    const auto hashed_class_name = fnv1a::hash(class_name);

    if (hashed_class_name == fnv1a::hash("CCSPlayerController")) {
      const auto player =
          i::m_game_entity_system->get<c_cs_player_controller *>(entity_handle);
      if (!player)
        continue;

      const auto player_pawn = player->get_player_pawn();
      if (!player_pawn)
        continue;

      if (!f::players::get_data(idx, player, player_pawn))
        continue;

      f::players::get_weapons(player_pawn);
      f::players::get_active_weapon(player_pawn);

      m_data["m_players"].push_back(m_player_data);
    } else if (hashed_class_name == fnv1a::hash("C_C4")) {
      const auto bomb = entity;
      f::bomb::get_carried_bomb(bomb);
    } else if (hashed_class_name == fnv1a::hash("C_PlantedC4")) {
      const auto planted_c4 = reinterpret_cast<c_planted_c4 *>(entity);
      f::bomb::get_planted_bomb(planted_c4);
    }
  }
}