/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
// #include "libloaderapi.h"
#include "windows.h"
#include "Gameh.h"

// NOTE: This is all about calling the function in the Xinput.h without the noticing from the compiler
#define PROCESSINPUT(name) void name(Game * g, PlatformP * p, bool* done)
typedef PROCESSINPUT(PROCESS_INPUT);
typedef PROCESS_INPUT(process_input);

#define UPDATE(name) void name(Game * g)
typedef UPDATE(UPDATE_);
typedef UPDATE_(update_);

#define RENDER(name) void name(PlatformP * p, Game* g)
typedef RENDER(RENDER_);
typedef RENDER_(render_);

#define CLOSE(name) void name(PlatformP * p, Game* g)
typedef CLOSE(CLOSE_);
typedef CLOSE_(close_);

HMODULE Game_Source_Dll;

bool Get_Game_Code(PlatformP* platform, Game* game){
    Game_Source_Dll = LoadLibraryA("game_source.dll");
    // PlatformP* platform = nullptr;
    // Game* game = nullptr;

    if (Game_Source_Dll) {
        platform = (PlatformP*)GetProcAddress(Game_Source_Dll, "PlatformP");
        game = (Game*)GetProcAddress(Game_Source_Dll, "Game");

        process_input = (ProcessInput* )GetProcAddress(Game_Source_Dll, "ProcessInput");
        update_ = (Update* )GetProcAddress(Game_Source_Dll, "Update");
        render_ = (Render* )GetProcAddress(Game_Source_Dll, "Render");
        close_ = (Close* )GetProcAddress(Game_Source_Dll, "Close");

        return true;
    } else {
        printf("Can not not code from DLL\n");
    }
}

int main( int argc, char* args[] )
{
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    PlatformP* platform = nullptr;
    Game* game = nullptr;
    
    Get_Game_Code(platform, game);
    platform = new PlatformP();
    game =  new Game();

    bool done = false;
    if(!Get_Game_Code(platform, game)){
        printf("Couldn't load game code\n");
    } else {
        if(!Start(platform, game)) {
            printf("Fail to init game\n");
        } else {
            printf("Init platform successfully\n");
            // NOTE:
            int count = 0;
            while(game->state != EMPTY) {

                if(count == 120){
                    FreeLibrary(Game_Source_Dll);
                    get_game_code(platform, game);
                } 
                process_input(game, platform, &done);
                update_(game);
                render_(platform, game);
                count++;
            }
            close_(platform, game);
            delete game;
            game = nullptr;
            delete platform;
            platform = nullptr;
            printf("End of Game, Thanks so much for playing my game\n");
        }        
    }

    return 0;
}

// Enable memory leak detection
#ifdef _DEBUG
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
