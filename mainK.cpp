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
        // NOTE:
            while(!done) {
                ProcessInput(&game);
                Update(&game, done);
                Render(&game);
            }
    }
return 0;
}
