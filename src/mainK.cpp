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
#define PROCESSINPUT(name) void name(Game * g,  bool* done)
typedef PROCESSINPUT(PROCESS_INPUT_);
// PROCESS_INPUT(processinputstub){};

#define UPDATE(name) void name(Game * g)
typedef UPDATE(UPDATE_);
// UPDATE_(updatestub){};

#define RENDER(name) void name( Game* g)
typedef RENDER(RENDER_);
// RENDER_(renderstub){};

#define CLOSE(name) void name( Game* g)
typedef CLOSE(CLOSE_);
// CLOSE_(closestub){};

static PROCESS_INPUT_* process_input_ ;
static UPDATE_* update_;
static RENDER_* render_;
static CLOSE_* close_;

HMODULE Game_Source_Dll;

bool Get_Game_Code(Game* game){
    Game_Source_Dll = LoadLibraryA("Gameh.dll");
    // PlatformP* platform = nullptr;
    // Game* game = nullptr;

    if (Game_Source_Dll) {
        game = (Game*)GetProcAddress(Game_Source_Dll, "Game");
        
       process_input_ = (PROCESS_INPUT_* )GetProcAddress(Game_Source_Dll, "ProcessInput");
       update_ = (UPDATE_* )GetProcAddress(Game_Source_Dll, "Update");
       render_ = (RENDER_* )GetProcAddress(Game_Source_Dll, "Render");
       close_ = (CLOSE_* )GetProcAddress(Game_Source_Dll, "Close");

        return true;
    } else {
        printf("Can not get code from DLL\n");
        return false;
    }
}

int main( int argc, char* args[] )
{
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Game* game = nullptr;
    
    Get_Game_Code( game);
    game =  new Game();

    bool done = false;
    if(!Get_Game_Code(game)){
        printf("Couldn't load game code\n");
    } else {
        if(!Start(game)) {
            printf("Fail to init game\n");
        } else {
            printf("Init platform successfully\n");
            // NOTE:
            int count = 0;
            while(game->state != EMPTY) {

                if(count == 120){
                    FreeLibrary(Game_Source_Dll);
                    Get_Game_Code(game);
                } 
                process_input_(game, &done);
                update_(game);
                render_(game);
                count++;
            }
            close_(game);
            delete game;
            game = nullptr;
            printf("End of Game, Thanks so much for playing my game\n");
        }        
    }

    return 0;
}

// Enable memory leak detection
#ifdef _DEBUG
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
