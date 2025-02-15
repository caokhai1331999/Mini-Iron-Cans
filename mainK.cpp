/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "Gameh.h"

int main( int argc, char* args[] )
{
    bool done = false;
    Game game = {};
    if(!Start(&game)) {
            printf("Fail to init game\n");
        } else {
        printf("Init game successfully\n");
        done = false;
            while(!done) {
                ProcessInput(&game, done);
                Update(&game);
                Render(&game);
            };
            done?printf("Out of Game Loop\n"):printf("Not out yet\n");
            close(game.tileSet,game.Platform);
            Close(&game);
            // Clean up
            };
return 0;
}
