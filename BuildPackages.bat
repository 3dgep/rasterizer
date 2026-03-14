@ECHO OFF

REM This script builds ZIP packages for the sample projects.
REM It detects the installed version of Visual Studio and uses
REM the appropriate CMake presets to configure, build, and package.

PUSHD "%~dp0"

SET VSWHERE="%~dp0\tools\vswhere\vswhere.exe"
SET CMAKE="cmake"
SET CPACK="cpack"

REM Detect latest version of Visual Studio.
FOR /F "usebackq delims=." %%i IN (`%VSWHERE% -latest -prerelease -requires Microsoft.VisualStudio.Workload.NativeGame -property installationVersion`) DO (
    SET VS_VERSION=%%i
)

IF %VS_VERSION% == 18 (
    SET CMAKE_PRESET=vs18
) ELSE IF %VS_VERSION% == 17 (
    SET CMAKE_PRESET=vs17
) ELSE (
    ECHO.
    ECHO ***********************************************************************
    ECHO *                                                                     *
    ECHO *                                ERROR                                *
    ECHO *                                                                     *
    ECHO ***********************************************************************
    ECHO No compatible version of Microsoft Visual Studio detected.
    ECHO Please make sure you have Visual Studio 2022 ^(or newer^) and the
    ECHO "Game Development with C++" workload installed before running this script.
    ECHO.
    PAUSE
    GOTO :Exit
)

REM Configure
ECHO.
ECHO ========================================
ECHO Configuring (%CMAKE_PRESET%)...
ECHO ========================================
%CMAKE% --preset %CMAKE_PRESET% -Wno-dev
IF %ERRORLEVEL% NEQ 0 (
    ECHO Configuration failed.
    PAUSE
    GOTO :Exit
)

REM Build Release
ECHO.
ECHO ========================================
ECHO Building Release...
ECHO ========================================
%CMAKE% --build --preset %CMAKE_PRESET%-release
IF %ERRORLEVEL% NEQ 0 (
    ECHO Build failed.
    PAUSE
    GOTO :Exit
)

REM Package 15-PixelAdventure
ECHO.
ECHO ========================================
ECHO Packaging 15-PixelAdventure...
ECHO ========================================
%CPACK% --preset %CMAKE_PRESET%-PixelAdventure
IF %ERRORLEVEL% NEQ 0 (
    ECHO Packaging 15-PixelAdventure failed.
    PAUSE
    GOTO :Exit
)

REM Package 16-Arkanoid
ECHO.
ECHO ========================================
ECHO Packaging 16-Arkanoid...
ECHO ========================================
%CPACK% --preset %CMAKE_PRESET%-Arkanoid
IF %ERRORLEVEL% NEQ 0 (
    ECHO Packaging 16-Arkanoid failed.
    PAUSE
    GOTO :Exit
)

ECHO.
ECHO ========================================
ECHO All packages built successfully.
ECHO ========================================

:Exit
PAUSE
POPD
