#if !defined(KSTATE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "Tile.h"
#include "xTank.h"

// TODO: CREATE DISPLAY MENU, CREATE STATES HERE

// NOTE: Using FSM (in this case the easiest and simplest is SWITCH CASE method)
enum STATE{
    MENU_IDLE, IN_GAME, PAUSE
};

enum OPTION{
    NEW_GAME, RESUME, OPTIONS, EXIT
};

#define KSTATE_H
#endif
