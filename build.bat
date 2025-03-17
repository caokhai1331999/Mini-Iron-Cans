@echo off
setlocal

set hr=%time:~0,2%
set hr=%hr: =0%
set min=%time:~3,2%
set min=%min: =0%
set sec=%time:~6,2%
set sec=%sec: =0%

set dd=%date:~-4,4%
set dd=%dd: =0%
set mm=%date:~-10,2%
set mm=%mm: =0%
set yr=%date:~-7,2%
set yr=%yr: =0%

REM Define paths to SDL include, library, and DLL directories
set DEFAULT_INCLUDE_DIR="..\include"
set COMMON_COMPILER_FLAG=/FC /Zi /MD /LD /EHsc
rem /EXPORT:Game /EXPORT:PlatformP
set EXPORT_=/EXPORT:ProcessInput /EXPORT:Update /EXPORT:Render /EXPORT:Close
set INHERENT_LIB=SDL2.lib SDL2main.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib shlwapi.lib Shell32.lib
set SDL_INCLUDE_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\include"
set SDL_LIB_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\lib\x64"
set SDL_DLL_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\bin\x64"

REM Define source files and output executable
set SRC_FILES=..\src\mainK.cpp
set OUT_EXE=sdl_project.exe

if not exist build (
    mkdir build
)

echo folder before pushd : %CD%
pushd build
echo folder after pushd : %CD%
del *.obj *.pdb *.lib

REM List all .cpp files in the current directory
setlocal enabledelayedexpansion

rem Compile the project
rem for %%f in (..\src\*.cpp) do (
rem    set FILES=!FILES! %%f
rem  )

REM Add DLL directory to PATH for runtime
set PATH=%DLL_DIR%;%PATH%

REM /subsystem:consoles
REM The reason why the compile the subsystem:consoles may solved the undebug problem. /fsanitize=address /FORCE:MULTIPLE /PDB:TANK_%hr%%min%%sec%_%dd%%mm%%yr%.pdb /LD /D _WIN64 ..\src\Gameh.cpp
rem del *.pdb
cl /LD ..\src\Gameh.cpp ..\src\GamePhysics.cpp ..\src\PlatformP.cpp ..\src\TileX.cpp ..\src\xTank.cpp ..\src\XTexture.cpp %COMMON_COMPILER_FLAG% /I %DEFAULT_INCLUDE_DIR% /I %SDL_INCLUDE_DIR% /link %EXPORT_% /PDB:main_%hr%%min%%sec%_%dd%%mm%%yr%.pdb /LIBPATH:%SDL_LIB_DIR% SDL2.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib shlwapi.lib Shell32.lib /DEBUG /OUT:Gameh.dll


cl ..\src\*.cpp %COMMON_COMPILER_FLAG% /I %DEFAULT_INCLUDE_DIR% /I %SDL_INCLUDE_DIR% /link  Gameh.dll %EXPORT_% /LIBPATH:%SDL_LIB_DIR% %INHERENT_LIB% /DEBUG /subsystem:console /OUT:%OUT_EXE%

if %ERRORLEVEL% EQU 0 (
   @echo Announce: compilation succeeded "(^ w ^)" at %time%
   
) else (
   @echo                                  @   @          
   @echo Announce: " compilation failed \_(-_-)_/ "
)

echo folder before popd: %CD%
popd
echo folder after popd: %CD%

pause
endlocal
