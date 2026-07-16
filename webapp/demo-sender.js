// Demo sender - simula o usermode.exe enviando dados para o radar
// Conecta em ws://localhost:22006/cs2_webradar (como o usermode.exe faria)
// Use o iniciar_demo.bat para rodar tudo automaticamente

import { WebSocket } from "ws";

const ws = new WebSocket("ws://localhost:22006/cs2_webradar");

ws.on("open", () => {
  console.log("Conectado ao servidor WebSocket!");
  console.log("Enviando dados de jogadores falsos...");

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

  // Envia dados a cada 100ms (igual o usermode.exe)
  setInterval(() => {
    for (const p of players) {
      p.m_pos_x += (Math.random() - 0.5) * 50;
      p.m_pos_y += (Math.random() - 0.5) * 50;
    }
    data.m_bomb.m_blow_time -= 0.1;
    if (data.m_bomb.m_blow_time < 0) data.m_bomb.m_blow_time = 40.5;

    ws.send(JSON.stringify(data));
  }, 100);
});

ws.on("error", (err) => {
  console.error("Erro ao conectar:", err.message);
  console.log("Certifique-se de que 'node ws/app.js' está rodando primeiro!");
});

ws.on("close", () => {
  console.log("Conexão fechada");
});