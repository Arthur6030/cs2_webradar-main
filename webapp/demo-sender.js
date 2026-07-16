// Demo sender - simulates usermode.exe data without CS2
// Run with: node demo-sender.js
import { WebSocketServer } from "ws";

const port = 22006;
const server = WebSocketServer({ port, path: "/cs2_webradar" });

console.log(`Demo sender listening on port ${port}`);

server.on("connection", (ws) => {
  console.log("Client connected");

  const players = [];
  const numPlayers = 10;

  for (let i = 0; i < numPlayers; i++) {
    const team = i % 2 === 0 ? 2 : 3;
    players.push({
      m_idx: i,
      m_team: team,
      m_pos_x: (Math.random() - 0.5) * 4000,
      m_pos_y: (Math.random() - 0.5) * 4000,
      m_health: Math.floor(Math.random() * 100) + 1,
      m_is_dead: Math.random() > 0.8,
      m_name: `Player${i}`,
    });
  }

  const data = {
    m_players: players,
    m_local_team: 2,
    m_map: "de_dust2",
    m_bomb: {
      m_blow_time: 40.5,
      m_is_defused: false,
      m_is_defusing: false,
      m_defuse_time: 0,
    },
  };

  const interval = setInterval(() => {
    for (const p of players) {
      p.m_pos_x += (Math.random() - 0.5) * 50;
      p.m_pos_y += (Math.random() - 0.5) * 50;
    }
    data.m_bomb.m_blow_time -= 0.1;
    if (data.m_bomb.m_blow_time < 0) data.m_bomb.m_blow_time = 40.5;

    ws.send(JSON.stringify(data));
  }, 100);

  ws.on("close", () => {
    clearInterval(interval);
    console.log("Client disconnected");
  });
});
