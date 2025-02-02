/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

// const global_variable int KEY_COUNT = SDL_NUM_SCANCODES;

int main( int argc, char* args[] )
{
    bool done = false;
    Game game = {};

    if(!Start(&game)){
            printf("Fail to init game\n");
        } else {
            while(!done){
                ProcessInput(&game);
                Update(&game);
                Render(&game);
            };
        }
        
}
