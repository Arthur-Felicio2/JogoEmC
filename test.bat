@echo off
echo ========================================
echo Executando testes do Starlight Games...
echo ========================================
echo.
"build\Release\rules_tests.exe"
if %ERRORLEVEL% NEQ 0 goto error
echo.
"build\Release\HandSprites_test.exe"
if %ERRORLEVEL% NEQ 0 goto error
echo.
"build\Release\ScoreStore_test.exe"
if %ERRORLEVEL% NEQ 0 goto error

echo.
echo ========================================
echo TODOS OS TESTES PASSARAM COM SUCESSO!
echo ========================================
goto end

:error
echo.
echo ========================================
echo FALHA EM UM OU MAIS TESTES!
echo ========================================

:end
pause
