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
    if(!Start(&game)){
            printf("Fail to init game\n");
        } else {
            while(!done){
                ProcessInput(&game, done);
                Update(&game);
                Render(&game);
                }
            // Clean up
            close(&game);
            };
        }
return 0;
}
