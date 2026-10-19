@echo off
cd /d "%~dp0"
if exist "build\Release\StarlightGames.exe" (
    start "" "build\Release\StarlightGames.exe"
) else if exist "build\Debug\StarlightGames.exe" (
    start "" "build\Debug\StarlightGames.exe"
) else (
    echo Executavel nao encontrado. Execute build.bat primeiro.
    pause
)

