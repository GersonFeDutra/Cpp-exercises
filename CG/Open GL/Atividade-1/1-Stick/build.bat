@echo off
setlocal ENABLEEXTENSIONS

:: Caminho do seu arquivo principal
set "SRC=main.cpp"
set "OUT=MyOpenGLApp.exe"

:: Verifica se o cmake está disponível
where cmake >nul 2>&1
if %ERRORLEVEL%==0 (
    echo [INFO] CMake encontrado. Iniciando build via CMake...

    if not exist build (
        mkdir build
    )
    cd build
    cmake .. -G "MinGW Makefiles"
    cmake --build .
    goto :EOF
)

:: Verifica se g++ está disponível (MinGW)
where g++ >nul 2>&1
if %ERRORLEVEL%==0 (
    echo [INFO] CMake não encontrado. Usando MinGW/g++ diretamente...

    echo [INFO] Compilando %SRC%...
    g++ %SRC% -o %OUT% -lglu32 -lopengl32 -lfreeglut -lgdi32 -luser32

    if %ERRORLEVEL%==0 (
        echo [SUCESSO] Compilado com sucesso: %OUT%
    ) else (
        echo [ERRO] Falha na compilação com g++
    )
    goto :EOF
)

:: Nenhuma opção encontrada
echo [ERRO] Nenhum compilador encontrado.
echo Instale o CMake ou o MinGW com g++ para compilar este projeto.
exit /b 1
