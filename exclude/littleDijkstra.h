#if !defined(LITTLEDIJKSTRA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <cmath>
#include <iostream>

using namespace std;

// TODO: First to give enough information for this guider: tank postion, distances
// then make it be able to produce the way to the user tank

struct Node{
    int name;
    int* path;
    Node(){
        path = new int;
    };
};

struct Graph{
    Node* node;
};

Node* CreateNode(int name ,int NumberOfpath);

#define LITTLEDIJKSTRA_H
#endif
