@echo off
echo ============================================
echo CS2 Web Radar - Tunnel Setup
echo ============================================
echo.
echo This will expose your WebSocket server to the internet
echo so you can access it from your phone.
echo.
echo Prerequisites:
echo   1. Install cloudflared from: https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/downloads/
echo   2. Or install ngrok from: https://ngrok.com/download
echo.
echo Choose your tunnel:
echo   1) Cloudflare Tunnel (recommended - free, no account needed for quick tunnels)
echo   2) ngrok
echo.
set /p choice="Enter choice (1 or 2): "

if "%choice%"=="1" (
    echo.
    echo Starting Cloudflare Tunnel...
    echo This will give you a URL like: https://XXXXX.trycloudflare.com
    echo.
    echo IMPORTANT: Copy the URL shown and put it in your phone browser!
    echo.
    cloudflared tunnel --url http://localhost:22006
) else if "%choice%"=="2" (
    echo.
    echo Starting ngrok tunnel...
    echo This will give you a URL like: https://XXXXX.ngrok.io
    echo.
    echo IMPORTANT: Copy the URL shown and put it in your phone browser!
    echo.
    ngrok http 22006
) else (
    echo Invalid choice!
    pause
    exit /b 1
)

pause