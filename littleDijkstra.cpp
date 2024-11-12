/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "littleDijkstra.h"

// NOTE: Something thing call node and paths between them will expose
// How to arrange them in effective way in order to call them easily

// TODO: Constantly update the position between tanks and give the bot guide the way to the user's tank

Node* CreateNode(int name ,int NumberOfPath){
    Node tempNode;
    int* tempPath = new int[NumberOfPath];
    tempNode.name = name;
    tempNode.path = tempPath;
    return &tempNode;
}
