# cs2_webradar - Anti-Ban

Radar web para CS2 (Counter-Strike 2) que funciona no navegador, incluindo no **celular**. 
Projeto modificado com **medidas anti-detecção** e deploy automático para **GitHub Pages**.

[![discord](https://img.shields.io/badge/discord-5865F2?style=flat&logo=discord&logoColor=white)](https://discord.gg/qEBpufFkXK)

## ⚠️ Aviso sobre VAC

Este projeto **apenas LÊ a memória** do CS2 usando `ReadProcessMemory`. Não injeta código, não modifica o jogo. Isso já é o método mais seguro. Adicionei camadas extras de proteção:

- ✅ Delays aleatórios entre leituras (evita padrões detectáveis)
- ✅ Ofuscação de strings de padrões no binário compilado
- ✅ Detecção de debugger (desliga se detectar)
- ✅ Código "junk" aleatório para confundir scanners
- ✅ Nomes de módulos e patterns ofuscados em runtime

> **Nenhum cheat é 100% seguro.** Use por sua conta e risco.

---

## 📱 Como ver no celular via GitHub Pages

### 1. Criar conta no GitHub (se não tiver)

1. Crie uma conta em [github.com](https://github.com)
2. Crie um repositório novo (ex: `cs2_webradar`)
3. Sobe todos os arquivos do projeto (pasta `webapp/` e `usermode/`)

### 2. Configurar GitHub Pages

1. No seu repositório, vá em **Settings > Pages**
2. Em "Source", selecione **GitHub Actions**
3. Pronto! Sempre que você der `git push` na branch `main`, o site vai fazer deploy automático

### 3. Expor o WebSocket do PC para internet

No seu PC (onde roda o CS2), você precisa expor a porta 22006:

**Opção A - Cloudflare Tunnel (recomendado, grátis):**
```bash
# Baixe cloudflared de: https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/downloads/
cloudflared tunnel --url http://localhost:22006
```
Copie a URL gerada (ex: `https://frango.trycloudflare.com`)

**Opção B - Port Forwarding no roteador:**
- Acesse `192.168.0.1` ou `192.168.1.1` no navegador
- Encontre "Port Forwarding" e adicione: porta `22006` TCP
- Descubra seu IP público em [ipinfo.io/ip](https://ipinfo.io/ip)

### 4. Acessar do celular

Abra no celular:
```
https://SEU_USUARIO.github.io/SEU_REPOSITORIO/?ip=SEU_IP_PUBLICO
```

Exemplo:
```
https://joao.github.io/cs2_webradar/?ip=177.54.32.10
```

Se usou Cloudflare Tunnel, coloque o IP que apareceu no terminal:
```
https://joao.github.io/cs2_webradar/?ip=frango.trycloudflare.com
```

---

## 🖥️ Como usar no PC (local)

### Requisitos
- [Node.js](https://nodejs.org/en/download)
- [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)
- [vcpkg](https://vcpkg.io/en/)

### Passos

1. **Instalar dependências do webapp:**
```bash
cd webapp
npm install
```

2. **Iniciar o servidor WebSocket + Vite:**
```bash
npm run dev
```

3. **Compilar o usermode (C++ DLL):**
   - Abra `usermode/usermode.sln` no Visual Studio
   - Vá em **Build > Build Solution** (ou Ctrl+Shift+B)

4. **Injetar no CS2:**
   - Inicie o CS2
   - Execute `usermode/Release/usermode.exe`

5. **Abrir o navegador:**
   - Vá para `http://localhost:5173`

---

## ⚙️ Configuração do IP

No arquivo `webapp/src/app.jsx`:

```javascript
// Opção 1: Local (só seu PC)
const USE_LOCALHOST = 1;

// Opção 2: IP fixo
const USE_LOCALHOST = 0;
const PUBLIC_IP = "177.54.32.10";

// Opção 3: Via URL (funciona no GitHub Pages)
// Acesse: http://localhost:5173/?ip=177.54.32.10
```

---

## 🛡️ Medidas Anti-Ban

| Medida | Como funciona |
|--------|--------------|
| **Delays aleatórios** | 80-200ms entre leituras (não fixo) |
| **Ofuscação de strings** | Patterns XOR no runtime |
| **Anti-debug** | Detecta debugger e desliga |
| **Junk code** | Instruções inúteis aleatórias |
| **Apenas leitura** | `ReadProcessMemory`, sem Write |
| **Sem injeção** | DLL externa, não modifica o CS2 |

---

## 📁 Estrutura do Projeto

```
cs2_webradar/
├── usermode/          # C++ DLL injetada no CS2
│   └── src/
│       ├── dllmain.cpp         # Loop principal com anti-ban
│       ├── common.hpp          # Strings ofuscadas
│       └── utils/antiban.hpp   # Medidas anti-detecção
├── webapp/            # Frontend React + WebSocket server
│   ├── src/app.jsx    # Radar com suporte a ?ip= na URL
│   ├── ws/app.js      # WebSocket server
│   ├── vite.config.js # Config para GitHub Pages
│   └── tunnel.bat     # Script de tunnel (Cloudflare/ngrok)
└── .github/workflows/ # Deploy automático para GitHub Pages
```

---

## 📄 Licença
Este projeto está licenciado sob [GPL-3.0](https://github.com/clauadv/cs2_webradar?tab=GPL-3.0-1-ov-file#readme)