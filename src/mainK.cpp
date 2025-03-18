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
#define START(name) bool name(Game * g)
typedef START(START_);
// PROCESS_INPUT(processinputstub){};

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

static START_* start_;
static PROCESS_INPUT_* process_input_ ;
static UPDATE_* update_;
static RENDER_* render_;
static CLOSE_* close_;

HMODULE Game_Source_Dll;

bool Get_Game_Code(){
    // NOTE: while game is running it lock Gameh.dll for hot loading code from it but still can be compile

    if (!CopyFile("Gameh.dll", "Gameh(copy).dll", false)){
        printf("Can not copy dll file\n");
    } else {
        Game_Source_Dll = LoadLibraryA("Gameh(copy).dll");        
    }


    if (Game_Source_Dll) {
<<<<<<< HEAD
       process_input_ = (PROCESS_INPUT_* )GetProcAddress(Game_Source_Dll, "ProcessInput");
       start_ = (START_* )GetProcAddress(Game_Source_Dll, "Start");
       update_ = (UPDATE_* )GetProcAddress(Game_Source_Dll, "Update");
       render_ = (RENDER_* )GetProcAddress(Game_Source_Dll, "Render");
       close_ = (CLOSE_* )GetProcAddress(Game_Source_Dll, "Close");
        printf("Get Code successfully from DLL\n");
=======
        process_input_ = (PROCESS_INPUT_* )GetProcAddress(Game_Source_Dll, "ProcessInput");
        update_ = (UPDATE_* )GetProcAddress(Game_Source_Dll, "Update");
        render_ = (RENDER_* )GetProcAddress(Game_Source_Dll, "Render");
        close_ = (CLOSE_* )GetProcAddress(Game_Source_Dll, "Close");
        printf("Get Code successfully from DLL with change\n");
>>>>>>> 3630db6128cda88172e81c9fb4cbe1327ee9ab2f
        return true;
    } else {
        printf("Can not get code from DLL with change\n");
        return false;
    }
}

void Unload_Game_Code(){
    FreeLibrary(Game_Source_Dll);
}

int main( int argc, char* args[] )
{
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Game* game = nullptr;
    game =  new Game();
    bool done = false;
    if(!Get_Game_Code()){
        printf("Couldn't load game code\n");
    } else {
        if(!start_(game)) {
            printf("Fail to init game\n");
        } else {
            printf("Init platform successfully not yet\n");
            // NOTE:
            int count = 0;
            while(game->state != EMPTY) {
                if(count == 120){
                    printf("Load and unload code\n");
                        Unload_Game_Code();
                        Get_Game_Code();
                        count = 0;
                        printf("Check for code change\n");
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
