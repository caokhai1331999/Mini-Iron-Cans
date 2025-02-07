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
    // NOTE: Put the fsm here to loop through states change
    // Do I need to jam all stuffs into game : update, render, input process
    if(!Start(&game)){
            printf("Fail to init game\n");
        } else {
            while(!done){
                ProcessInput(&game, done);
                Update(&game);
                Render(&game);
                }
            };
        }        
}
