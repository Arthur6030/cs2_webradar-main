@echo off
title CS2 WebRadar
chcp 65001 >nul

echo ============================================
echo         CS2 WebRadar - INICIAR TUDO
echo ============================================
echo.

:: Ir para pasta do projeto
cd /d "%~dp0"

:: Copiar config.json para a pasta do exe
copy /Y usermode\config.json usermode\release\config.json >nul 2>&1
echo [1/5] Config copiado

:: Iniciar servidor WebSocket
echo [2/5] Iniciando servidor WebSocket...
start "WebSocket Server" cmd /c "cd /d webapp && node ws/app.js"
timeout /t 2 >nul

:: Iniciar Vite (site)
echo [3/5] Iniciando site Vite...
start "Vite Server" cmd /c "cd /d webapp && npx vite"
timeout /t 3 >nul

:: Abrir o navegador
echo [4/5] Abrindo navegador...
start http://localhost:5173/

:: (Opcional) Executar usermode.exe
echo [5/5] Iniciando usermode.exe (leitor do CS2)...
echo.
echo Se o CS2 estiver aberto, o radar vai receber dados reais.
echo Se nao, feche essa janela e execute "iniciar_demo.bat"
echo.
timeout /t 3 >nul
cd /d "%~dp0usermode\release"
start "CS2 Radar" cmd /c "usermode.exe & pause"

echo.
echo Pronto! O radar está em: http://localhost:5173
echo.
pause