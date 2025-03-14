@echo off
setlocal

REM Define paths to SDL include, library, and DLL directories
set DEFAULT_INCLUDE_DIR="..\include"
set SDL_INCLUDE_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\include"
set SDL_LIB_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\lib\x64"
set SDL_DLL_DIR="C:\Users\Cao Khai\Downloads\External_Libraries\SDL2\SDL2-2.30.2\bin\x64"

REM Define source files and output executable
set SRC_FILES=mainX.cpp
set OUT_EXE=sdl_project.exe

if not exist build (
    mkdir build
)

echo folder before pushd : %CD%
pushd build
echo folder after pushd : %CD%
rem del *.obj *.pdb 

REM List all .cpp files in the current directory
setlocal enabledelayedexpansion

REM Compile the project
for %%f in (..\src\*.cpp) do (
   set FILES=!FILES! %%f
 )

REM Add DLL directory to PATH for runtime
set PATH=%DLL_DIR%;%PATH%

REM /subsystem:consoles
REM The reason why the compile the subsystem:consoles may solved the undebug problem. /fsanitize=address /FORCE:MULTIPLE
cl %FILES% /FC /Zi /EHsc  /RTC1 /I %DEFAULT_INCLUDE_DIR% /I %SDL_INCLUDE_DIR% /link /LIBPATH:%SDL_LIB_DIR% /subsystem:console  SDL2.lib SDL2main.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib shlwapi.lib Shell32.lib /FORCE:MULTIPLE /DEBUG /OUT:%OUT_EXE%

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
