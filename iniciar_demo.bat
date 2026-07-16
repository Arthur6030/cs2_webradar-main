@echo off
title CS2 WebRadar - MODO DEMO
chcp 65001 >nul

echo ============================================
echo    CS2 WebRadar - MODO DEMO (sem CS2)
echo ============================================
echo.
echo Esse modo envia dados FALSOS de jogadores
echo para voce testar o radar no navegador!
echo.

cd /d "%~dp0"

:: Iniciar servidor WebSocket
echo [1/3] Iniciando servidor WebSocket...
start "WebSocket Server" cmd /c "cd /d webapp && node ws/app.js"
timeout /t 2 >nul

:: Iniciar Vite (site)
echo [2/3] Iniciando site Vite...
start "Vite Server" cmd /c "cd /d webapp && npx vite"
timeout /t 3 >nul

:: Iniciar demo sender (simula dados do CS2)
echo [3/3] Iniciando demo sender...
start "Demo Sender" cmd /c "cd /d webapp && node demo-sender.js"

:: Abrir navegador
timeout /t 1 >nul
start http://localhost:5173/

echo.
echo ============================================
echo   Pronto! Radar aberto em: http://localhost:5173
echo.
echo   Para acessar pelo celular, veja as
echo   instrucoes no arquivo tunnel.bat
echo ============================================
echo.
pause