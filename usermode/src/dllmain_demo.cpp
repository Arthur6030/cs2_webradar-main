#include "pch.hpp"
#include <nlohmann/json.hpp>

bool main() {
  bool running = true;

  // Wait for CS2 (but don't require it for demo)
  printf("[info] connecting to the web socket...\n");

  ix::WebSocket web_socket;
  web_socket.setUrl("ws://localhost:22006/cs2_webradar");

  std::mutex mtx;
  bool connected = false;

  web_socket.setOnMessageCallback([&](const ix::WebSocketMessagePtr &msg) {
    if (msg->type == ix::WebSocketMessageType::Open) {
      connected = true;
      printf("[info] connected to the web socket\n");
    }
  });

  web_socket.start();

  // Wait for connection or timeout
  for (int i = 0; i < 50 && !connected; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if (!connected) {
    printf("[error] failed to connect to the web socket\n");
    return false;
  }

  // Demo data
  nlohmann::json data;
  data["m_players"] = {{{"m_idx", 1},
                        {"m_team", 2},
                        {"m_pos_x", -1000},
                        {"m_pos_y", 2000},
                        {"m_health", 100}},
                       {{"m_idx", 2},
                        {"m_team", 3},
                        {"m_pos_x", 3000},
                        {"m_pos_y", -4000},
                        {"m_health", 75}}};
  data["m_local_team"] = 2;
  data["m_map"] = "de_dust2";
  data["m_bomb"] = {{"m_blow_time", 40.5},
                    {"m_is_defused", false},
                    {"m_is_defusing", false},
                    {"m_defuse_time", 0}};

  printf("[info] sending demo data...\n");

  while (running) {
    web_socket.send(data.dump());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return true;
}