#if !defined(KSTATE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "stdint.h"

// TODO: CREATE DISPLAY MENU, CREATE STATES HERE

// NOTE: Using FSM (in this case the easiest and simplest is SWITCH CASE method)

enum STATE{
    // NOTE: How to tell the new game state apart from the current one
    EMPTY, MENU_INIT, GAME_NEW, GAME_RELOADED, PAUSE
};

enum MENUCHOICE{
    NONE, NEW_GAME, RESUME, OPTIONS, EXIT
};

#define KSTATE_H
#endif
