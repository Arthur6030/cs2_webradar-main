#include "pch.hpp"
#include "utils/antiban.hpp"

bool main() {
  // Anti-debug: exit if debugger is present
  if (antiban::is_debugged()) {
    LOG_ERROR("debugger detected, exiting");
    return {};
  }

  config_data_t config_data = {};
  printf("[debug] starting setup...\n");
  INIT_STEP("config system", cfg::setup(config_data));
  printf("[debug] config done, ip=%s\n", config_data.m_ip.c_str());
  INIT_STEP("memory", m_memory->setup());
  printf("[debug] memory done\n");
  printf("[debug] starting interfaces setup...\n");
  INIT_STEP("interfaces", i::setup());
  printf("[debug] interfaces done\n");
  printf("[debug] starting schema setup...\n");
  INIT_STEP("schema", schema::setup());
  printf("[debug] schema done\n");

  ix::initNetSystem();
  LOG_INFO("winsock initialization completed");

  printf("[info] waiting for cs2.exe to start...\n");
  while (true) {
    const auto pid = m_memory->get_process_id("cs2.exe");
    if (pid.has_value()) {
      printf("[info] cs2.exe detected (pid: %d)\n", pid.value());
      break;
    }
    printf("[debug] cs2.exe not found, waiting...\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  printf("[info] re-setting up memory after cs2 detected...\n");
  m_memory->setup();
  printf("[debug] memory re-setup done\n");

  const auto formatted_address =
      std::format("ws://{}:22006/cs2_webradar", config_data.m_ip);

  static ix::WebSocket web_socket;
  std::mutex handshake_mutex;
  std::condition_variable handshake_cv;
  bool connected = false;
  bool failed = false;

  web_socket.setUrl(formatted_address);
  web_socket.setOnMessageCallback([&](const ix::WebSocketMessagePtr &msg) {
    if (msg->type == ix::WebSocketMessageType::Open) {
      {
        std::lock_guard lock(handshake_mutex);
        connected = true;
      }
      handshake_cv.notify_one();
      LOG_INFO("connected to the web socket ('%s')", formatted_address.c_str());
    } else if (msg->type == ix::WebSocketMessageType::Error) {
      {
        std::lock_guard lock(handshake_mutex);
        failed = true;
      }
      handshake_cv.notify_one();
      LOG_ERROR("failed to connect to the web socket ('%s')",
                formatted_address.c_str());
    }
  });
  printf("[info] connecting to web socket...\n");
  web_socket.start();

  {
    std::unique_lock lock(handshake_mutex);
    handshake_cv.wait(lock, [&] { return connected || failed; });
  }

  if (!connected) {
    LOG_ERROR("failed to connect to web socket, exiting");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return {};
  }

  // Random initial delay to avoid pattern detection
  antiban::random_sleep(500, 2000);

  for (;;) {
    // Anti-debug check every iteration
    if (antiban::is_debugged()) {
      LOG_ERROR("debugger detected during runtime, exiting");
      break;
    }

    sdk::update();
    f::run();
    web_socket.send(f::m_data.dump());

    // Random sleep between 80-200ms instead of fixed 100ms
    antiban::random_sleep(80, 200);
    antiban::junk_code();
  }

  return true;
}
