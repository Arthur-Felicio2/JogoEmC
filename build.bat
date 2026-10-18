@echo off
setlocal
echo ========================================
echo Compilando Starlight Games...
echo ========================================

set "CMAKE_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if exist "%CMAKE_PATH%" (
    "%CMAKE_PATH%" -B build -G "Visual Studio 17 2022" -A x64
    "%CMAKE_PATH%" --build build --config Release --target StarlightGames
    "%CMAKE_PATH%" --build build --config Release --target rules_tests
    "%CMAKE_PATH%" --build build --config Release --target HandSprites_test
    "%CMAKE_PATH%" --build build --config Release --target ScoreStore_test
) else (
    cmake -B build -G "Visual Studio 17 2022" -A x64
    cmake --build build --config Release --target StarlightGames
    cmake --build build --config Release --target rules_tests
    cmake --build build --config Release --target HandSprites_test
    cmake --build build --config Release --target ScoreStore_test
)

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacao concluida com sucesso!
    echo Execute run.bat para iniciar o jogo.
    echo ========================================
) else (
    echo.
    echo Erro na compilacao.
)
pause
