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
    bool done = false;
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);;
    Game* game = nullptr;
    game = new Game;
    if(!Start(game)) {
            printf("Fail to init game\n");
        } else {
        printf("Init game successfully\n");
        // NOTE:
            while(!done) {
                ProcessInput(game);
                Update(game, done);
                Render(game);
                if (done){
                    delete game;
                    game = nullptr;
                }
            }
    }
return 0;
}

// Enable memory leak detection
#ifdef _DEBUG
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
