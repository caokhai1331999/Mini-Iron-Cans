/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "libloaderapi.h"
#include "Gameh.h"

// NOTE: This is all about calling the function in the Xinput.h without the noticing from the compiler

// #define GET_PLATFORM(name) void name(PlatformP* platform)
// typedef GET_GAME_PLATFORM(get_platform);

// #define GET_GAME(name) void name(Game* game)
// typedef GET_GAME_PLATFORM(get_game);

// get_platform(getgameplatform) {
// };

// void Get_Game_Code(PlatformP* platform, Game* game){
//     HMODULE Game_Source_Dll = LoadLibraryA("game_source.dll");
//     if (Game_Source_Dll) {
//         platform = (PlatformP*)GetProcAddress(Game_Source_Dll, "Platform");
//         game = (Game*)GetProcAddress(Game_Source_Dll, "Game");
//         return true;
//     } else {
//         printf("Can not not code from DLL\n");
//     }
// }

int main( int argc, char* args[] )
{
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    PlatformP* platform = nullptr;
    Game* game = nullptr;
    platform = new PlatformP();
    game =  new Game();        


    // if(Get_Game_Code(platform, game)){
    // }


    bool done = false;

    if(!Start(platform, game)) {
            printf("Fail to init game\n");
        } else {
        printf("Init platform successfully\n");
        // NOTE:
            while(game->state != EMPTY) {                
                ProcessInput(game, platform, &done);
                Update(game);
                Render(platform, game);
            }
            Close(platform, game);
            delete game;
            game = nullptr;
            delete platform;
            platform = nullptr;
            printf("End of Game, Thanks so much for playing my game\n");
    }
return 0;
}

// Enable memory leak detection
#ifdef _DEBUG
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
