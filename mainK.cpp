/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Gameh.h"


int main( int argc, char* args[] )
{
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // PlatformP* Platform = nullptr;
    // Platform = new PlatformP;
    // Game* game = nullptr;
    // game = new Game;
    PlatformP Platform = {};
    Game game = {};    
    bool done = false;
    // Platform = new PlatformP;
    if(!Start(&Platform, &game)) {
            printf("Fail to init &game\n");
        } else {
        printf("Init &game successfully\n");
        // NOTE:
            while(game.state != EMPTY) {
                
                ProcessInput(&game, &done);
                Update(&game);
                Render(&Platform, &game);
            }
            Close(&Platform, &game);
    // delete game;
    // game = nullptr;
    // delete Platform;
    // Platform = nullptr;
    }
return 0;
}

// Enable memory leak detection
#ifdef _DEBUG
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
