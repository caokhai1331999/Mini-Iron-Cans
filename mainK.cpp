/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "KState.h"
#include "Gameh.h"

int main( int argc, char* args[] )
{
    bool done = false;
    Game game = {};
    // NOTE: Put the fsm here to loop through states change
    
    if(!Start(&game)){
            printf("Fail to init game\n");
        } else {
        STATE currrentState = MENU_IDLE;
        OPTION Option = NONE;
        while(currentState != EMPTY){
            // NOTE: Get option here
            // 
            
            switch(currentState){
                case MENU_IDLE :
                    displayMenu(&game);
                    ProcessInput(&game);
                    break;
                    
                case IN_GAME :
                    Update(&game);
                    break;

                case IN_GAME : ;
                    break;

                case IN_GAME : ;
                    break;
                    
                    }
        }
            while(!done){
                ProcessInput(&game);
            };
        }
        
}
